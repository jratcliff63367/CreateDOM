#include "Utilities/Tools.h"
#include "NsRunProgram.h"
#include "NsSimpleString.h"
#include "NsStringUtils.h"
#include "NvPreprocessor.h"
#include "NsXorString.h"
#include "NsLogging.h"
#include "NsFile.h"
#include "NsProgressBar.h"
#include "NsTime.h"
#include "NsFileSystem.h"
#include <string.h>

#if NV_WINDOWS_FAMILY
#include "windows/NsWindowsInclude.h"
#include "Shlwapi.h"
#pragma  comment(lib, "Shlwapi.lib")
#pragma warning(disable:4996)
#include <direct.h>
#include <strsafe.h>
#include "NsMyMessageBox.h"
#pragma warning(push)
#pragma warning(disable:4244 4267 4091 4100 4505 4189 4703 4701)
#define STB_DEFINE
#include "stb.h"
#pragma warning(pop)
#endif

namespace lingcod
{

#if NV_WINDOWS_FAMILY

        const char *getBasePath()
        {
            static char fileName[512];
            GetModuleFileNameA(NULL, fileName, 512);

    #if 0 // This is not necessary and is causing an assert 
            // store executable CRC32
            uint32_t endOfFile;
            nvidia::FS_FILE* exefp = nvidia::FileSystem::getInstance().open(fileName, endOfFile, nvidia::FM_OS_READ_ONLY);
            NV_ASSERT(exefp, "executable not found");
            if (exefp)
            {
                mCRC32 = nvidia::CRCFile(exefp, endOfFile, 0);
                exefp->release();
            }
    #endif

            char *slash = nvidia::getLastSlash(fileName);
            NV_ASSERT(slash, "AssertFailure");
            *slash = 0;

            bool isLingcodPak = strstr(fileName, "\\tools\\LingcodElf") ? true : false;

            slash = nvidia::getLastSlash(fileName);
            NV_ASSERT(slash, "AssertFailure");
            *slash = 0;

            slash = nvidia::getLastSlash(fileName);
            NV_ASSERT(slash, "AssertFailure");
            *slash = 0;


            if (!isLingcodPak)
            {
                slash = nvidia::getLastSlash(fileName);
                NV_ASSERT(slash, "AssertFailure");
                *slash = 0;
            }

            return fileName;
        }

        const char *getP4Root()
        {
            return getenv("P4ROOT");
        }

        bool CanOpenBroadwayManaul()
        {
            return true;
        }

        nvidia::SimpleString GetRegistry(const char* path, const char *key)
        {
            LONG result;
            DWORD dwType = REG_SZ;
            HKEY hKey = 0;
            char value[1024];
            DWORD value_length = 1024;

            result = RegOpenKey(HKEY_LOCAL_MACHINE, path, &hKey);
            if (result == ERROR_SUCCESS)
            {
                result = RegQueryValueEx(hKey, key, NULL, &dwType, (LPBYTE)&value, &value_length);

                RegCloseKey(hKey);

                if (result == ERROR_SUCCESS)
                {
                    return nvidia::SimpleString(value);
                }
            }
            return nvidia::SimpleString();
        }

        bool OpenBroadwayManual(int page)
        {
            char manualPath[512];
            
            const char *p4Root = getP4Root();

            if (!p4Root)
            {
                return false;
            }
            nvidia::stringFormat(manualPath, "%s\\odin\\northport\\docs\\BroadwayUserManual.pdf", p4Root);
            const char *chromeRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\chrome.exe";

            nvidia::SimpleString chromePath = GetRegistry(chromeRegPath, nullptr);

            if (chromePath.empty() == true) return false;          // Chrome not installed.

            char pwd[MAX_PATH];
            char outPath[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, pwd);
            PathCombine(outPath, pwd, manualPath);

            char pageBuffer[1024];
            nvidia::stringFormat(pageBuffer, sizeof(pageBuffer), "#page=%d", page);

            // Replace \ with / and append file:/// to it.
            nvidia::SimpleString path = nvidia::SimpleString("file:///") + nvidia::SimpleString(outPath) + nvidia::SimpleString(pageBuffer);
            for (uint32_t i = 0; i < path.size(); i++)
            {
                if (path[i] == '\\') path[i] = '/';
            }

            nvidia::runProgram(chromePath.c_str(), path.c_str(), nullptr, true, false);

            return true;
        }

#else
        bool CanOpenBroadwayManaul()
        {
            return false;
        }

        bool OpenBroadwayManual(int page)
        {
            return false;
        }
#endif

#if NV_WINDOWS_FAMILY

        bool launchNotepad(const char *sourceFile, uint32_t lineno)
        {
            bool ret = false;

            NV_UNUSED(sourceFile);
            NV_UNUSED(lineno);
            char scratch[512];
            GetModuleFileNameA(0, scratch, 512);
            char *slash = nvidia::getLastSlash(scratch);
            if (slash)
            {
                *slash = 0;
                slash = nvidia::getLastSlash(scratch);
                if (slash)
                {
                    *slash = 0;
                    slash = nvidia::getLastSlash(scratch);
                    if (slash)
                    {
                        *slash = 0;
                        // ok... we have the base directory here...
                        char notepad[512];
                        nvidia::stringFormat(notepad, 512, "%s\\tools\\notepad++\\notepad++.exe", scratch);
                        char commandLine[512];
                        nvidia::stringFormat(commandLine, 512, "-n%d %s", lineno, sourceFile);
                        nvidia::runProgram(notepad, commandLine, nullptr, true, false);
                        ret = true;
                    }
                }
            }


            return ret;
        }

        bool runELF2ALF(const char *elfName, const char *alfName, bool includeDebugSourceLine)
        {
            bool ret = false;

            char lingcodElf[512];
            // ok... we have the base directory here...
            nvidia::stringFormat(lingcodElf, 512, "%s\\tools\\LingcodElf\\LingcodElf.exe", getBasePath());
            char commandLine[512];
            nvidia::stringFormat(commandLine, 512, "%s %s%s", elfName, alfName, includeDebugSourceLine ? "" : XorString(" -nodebug"));
            ret = nvidia::runProgram(lingcodElf, commandLine, nullptr, false, true);

            return ret;
        }

        static bool CharToWide(const char *source, void *dest,int32_t maxlen)
        {
            bool ret = false;

            ret = true;
            mbstowcs((wchar_t *)dest, source, maxlen);

            return ret;
        }

        static void add(wchar_t *dest, const wchar_t *src, uint32_t &index)
        {
            if (src)
            {
                size_t len;
                StringCchLengthW(src, 512, &len);
                for (uint32_t i = 0; i < len; i++)
                {
                    wchar_t c = *src++;
                    dest[index++] = c;
                }
            }
            else
            {
                wchar_t c = 0;
                dest[index++] = c;
            }
        }

        const char * getFileName(const char *fileType, const char *initial, const char *description, bool saveMode) // allows the application the opportunity to present a file save dialog box.
        {
            const char *ret = initial;

            const char *extension = fileType;

            static int32_t sWhichFileType = 1;
            char curdir[512];
            getcwd(curdir, 512);
            wchar_t buffer[257] = L"";

            OPENFILENAMEW f;

            memset(&f, 0, sizeof(OPENFILENAME));
            f.lStructSize = sizeof(OPENFILENAME);
            f.hwndOwner = GetActiveWindow();
            f.lpstrFile = buffer;
            f.nMaxFile = 256;
            f.nFilterIndex = sWhichFileType;

            char exportName[512] = { 0 };
            if (initial)
                strcpy(exportName, initial);

            assert(extension);

            wchar_t _filter[512];

            wchar_t _ext[512];
            wchar_t _desc[512];

            CharToWide(description, _desc, 512);
            CharToWide(extension, _ext, 512);

            uint32_t index = 0;
            _filter[0] = 0;

            add(_filter, _desc, index);
            add(_filter, L" (*", index);
            add(_filter, _ext, index);
            add(_filter, L")", index);
            add(_filter, 0, index);
            add(_filter, L"*", index);
            add(_filter, _ext, index);
            add(_filter, 0, index);
            add(_filter, 0, index);
            add(_filter, 0, index);


            f.lpstrFilter = _filter;
            f.lpstrTitle = _desc;

            f.lpstrInitialDir = NULL;

            mbstowcs(buffer, exportName, 512);

            int32_t ok = 0;
            if (saveMode)
                ok = GetSaveFileNameW(&f);
            else
                ok = GetOpenFileNameW(&f);

            if (ok)
            {
                // save off which file type they chose for next time
                sWhichFileType = f.nFilterIndex;

                // first change dir, and get the name in "normal" char format
                chdir(curdir);

                static char tmp[512];
                wcstombs(tmp, buffer, 512);

                bool ok = true;

                if (saveMode)
                {
                    uint32_t flen;
                    nvidia::FS_FILE *fph = nvidia::FileSystem::getInstance().open(tmp, flen, nvidia::FM_OS_READ_ONLY);
                    if (fph)
                    {
                        fph->release();
                        nvidia::MessageBoxResult result = nvidia::myMessageBoxYesNoCancel("Are you sure you want to overwrite the file '%s'?", tmp);
                        if (result != nvidia::MBR_YES)
                        {
                            ok = false;
                        }
                    }
                }

                if (ok)
                    ret = tmp;
                else
                    ret = nullptr;
            }
            else
            {
                ret = nullptr;
            }
            chdir(curdir);
            return ret;
}

TextureResult convertTexture(const char *sourceTexture,     // The fully qualified path name of the source texture
    const char *destTexture,       // The fully qualified path name of the dest texture
    const char *compressionFormat,// command line switches to pass to the nvcompress tool
    bool forceRebuildAllTextures)
{
    TextureResult ret = TR_SUCCESS;

    // First, make sure the source texture exists
    uint32_t flen;
    nvidia::FS_FILE *fptest = nvidia::FileSystem::getInstance().open(sourceTexture, flen, nvidia::FM_OS_READ_ONLY);
    if ( fptest == nullptr )
    {
        ret = TR_FAILED_NO_SOURCE;  // Return error code if we cannot find the source texture
    }
    else
    {
        fptest->release();
        // Now locate the texture conversion tool; relative to the executable directory
        char toolsDir[512];

        nvidia::stringFormat(toolsDir, "%s\\tools\\TextureTools", getBasePath());

        char toolsFile[512];
        nvidia::stringFormat(toolsFile, "%s\\nvcompress.exe", toolsDir);
        // See if the NVIDIA Texture Tool exists
        uint32_t flen;
        nvidia::FS_FILE *fph = nvidia::FileSystem::getInstance().open(toolsFile, flen, nvidia::FM_OS_READ_ONLY);
        if (fph)
        {
            fph->release();
            // If we are not ignoring timestamps; then see if the destination file already exists and, if it does, whether the source file is newer or not
            if (!forceRebuildAllTextures)
            {
                // See if the destination texture already exists..
                uint32_t flen;
                fptest = nvidia::FileSystem::getInstance().open(destTexture, flen, nvidia::FM_OS_READ_ONLY);
                if (fptest)
                {
                    fptest->release();
                    // Get the time date stamp of the source and the destination file
                    uint64_t oldFileTime = nvidia::getFileTime(sourceTexture);
                    uint64_t newFileTime = nvidia::getFileTime(destTexture);
                    if (oldFileTime < newFileTime)
                    {
                        ret = TR_SKIPPED; // skipping file because it's already been converted.
                    }
                }
            }
            // If we haven't decided to skip this file; then let's do the conversion
            if (ret == TR_SUCCESS)
            {
                uint32_t flen;
                fptest = nvidia::FileSystem::getInstance().open(destTexture, flen, nvidia::FM_OS_READ_ONLY);  // see if the destination file already exists...
                if (fptest)
                {
                    fptest->release(); // If it does, then we delete it before we launch the conversion tool
                    BOOL ok = DeleteFileA(destTexture);// first we delete the destination file
                    if (!ok)
                    {
                        ret = TR_FAILED_DEST_FILE; // couldn't delete the file for some unknown reason.
                    }
                }
                // If we are still good to continue...
                if ( ret == TR_SUCCESS )
                {
                    // Create the command line arguments to pass to the texture conversion tool
                    char commandLine[512];
                    nvidia::stringFormat(commandLine, 512, "%s \"%s\" \"%s\"", compressionFormat, sourceTexture, destTexture);
                    nvidia::runProgram(toolsFile, commandLine, nullptr, false, true);
                    uint32_t flen;
                    fptest = nvidia::FileSystem::getInstance().open(destTexture, flen, nvidia::FM_OS_READ_ONLY);
                    if (fptest)
                    {
                        size_t len = size_t(flen);
                        fptest->release();
                        if (len < 8)
                        {
                            DeleteFileA(destTexture);// first we delete the destination file
                            ret = TR_FAILED_OUTPUT;
                        }
                    }
                    else
                    {
                        ret = TR_FAILED_OUTPUT; // didn't generate the destination file for some reason!
                    }
                }
            }
        }
        else
        {
            NV_ALWAYS_ASSERT("Failed to locate the 'nvcompress.exe' tool??");
            ret = TR_FAILED_NO_TOOL;
        }
    }
    return ret;
}


uint32_t convertTextures(const char *_sourceTextures, const char *_destTextures, const char *compressionFormat,bool forceRebuildAllTextures,TextureResult &result)
{
    uint32_t ret = 0;


    nvidia::Time t;

    nvidia::ProgressBar *pb = nullptr;

    result = TR_SKIPPED;
    char sourceTextures[512];
    char destTextures[512];
    strncpy(sourceTextures, _sourceTextures, 512);
    strncpy(destTextures, _destTextures, 512);
    nvidia::normalizePathBackSlash(sourceTextures);
    nvidia::normalizePathBackSlash(destTextures);

    _mkdir(sourceTextures);
    _mkdir(destTextures);

    char **files = stb_readdir_recursive((char *)sourceTextures, (char *)"*.png");

    if (files)
    {
        uint32_t slen = uint32_t(strlen(sourceTextures));
        int count = stb_arr_len(files);

        DEBUG_LOG(nvidia::DC_GENERAL, "Processing %d source texture files.\n", count);
        for (int i = 0; i < count; i++)
        {
            if (pb == nullptr)
            {
                auto dtime = t.peekElapsedSeconds();
                if (dtime > 1.0f)
                {
                    pb = nvidia::ProgressBar::create(50, 50, 500, 250, "Converting Replacement PNG textures to DDS format.", true, 0, 1, 0, "Scanning for replacement textures.");
                }
            }
            char *fname = files[i];
            char scratch[512];
            strncpy(scratch, fname, 512);
            nvidia::normalizePathBackSlash(scratch);
            char *slash = nvidia::lastSlash(scratch);
            NV_ASSERT(slash);
            if (slash)
            {
                *slash = 0;
                const char *baseFileName = slash + 1;
                if (nvidia::strnicmp(scratch, sourceTextures, slen) == 0)
                {
                    const char *subDir = scratch;
                    subDir += slen;
                    char newDir[512];
                    char newFile[512];
                    nvidia::stringFormat(newDir, "%s\\%s", destTextures, subDir);
                    _mkdir(newDir); // make the directory if it doesn't already exist...
                    nvidia::stringFormat(newFile, "%s\\%s", newDir, baseFileName);
                    char *scan = (char *)nvidia::stristr(newFile, ".png");
                    NV_ASSERT(scan);
                    if (scan)
                    {
                        *scan = 0;
                        strcat(newFile, ".dds");
                    }
                    if ( pb )
                    {
                        const char *slash = nvidia::lastSlash(fname);
                        if (slash)
                        {
                            slash++;
                            float progress = float(i) / float(count);
                            bool keepGoing = pb->updateProgress(progress, "Converting replacement texture '%s'", slash);
                            if (!keepGoing)
                            {
                                break;
                            }
                        }
                    }

                    result = lingcod::convertTexture(fname, newFile,compressionFormat,forceRebuildAllTextures);

                    switch (result)
                    {
                        case lingcod::TR_SUCCESS:
                            ret++;
                            DEBUG_LOG(nvidia::DC_GENERAL, "Successfully converted input texture (%s) to output texture: %s\n", fname, newFile);
                            break;
                        case lingcod::TR_SKIPPED:
                            DEBUG_LOG(nvidia::DC_GENERAL, "Skipped input texture (%s)\n", fname);
                            break;
                        case lingcod::TR_FAILED_NO_TOOL:
                            DEBUG_LOG(nvidia::DC_GENERAL, "Failed to locate texture conversion tool\n");
                            break;
                        case lingcod::TR_FAILED_NO_SOURCE:
                            DEBUG_LOG(nvidia::DC_GENERAL, "Failed to locate source texture: %s\n", fname);
                            break;
                        case lingcod::TR_FAILED_OUTPUT:
                            {
                                DEBUG_LOG(nvidia::DC_GENERAL, "Failed to generate output texture file '%s'\n", newFile);
                            }
                            break;
                        case lingcod::TR_FAILED_DEST_FILE:
                            DEBUG_LOG(nvidia::DC_GENERAL, "Failed to delete the previous destination file '%s'\n", newFile);
                            break;
                    }
                    if (result == lingcod::TR_SUCCESS || result == lingcod::TR_SKIPPED)
                    {
                    }
                    else
                    {
                        nvidia::MessageBoxResult r = nvidia::myMessageBoxYesNoCancel("Failed to convert texture file '%s'\nDo you wish to continue?\n", fname);
                        if (r != nvidia::MBR_YES)
                        {
                            break;
                        }
                    }
                }
                else
                {
                    NV_ALWAYS_ASSERT();
                }
            }
        }
        stb_readdir_free(files);
    }
    if (pb)
    {
        pb->release();
    }
    return ret;
}


#else

        bool runELF2ALF(const char *elfName, const char *alfName, bool includeDebugSourceLine)
        {
            bool ret = false;

            return ret;
        }


        bool launchNotepad(const char *sourceFile, uint32_t lineno)
        {
            NV_UNUSED(sourceFile);
            NV_UNUSED(lineno);
            return false;
        }

        const char * getFileName(const char *fileType,
            const char *initial,
            const char *description,
            bool saveMode)
        {
            NV_UNUSED(fileType);
            NV_UNUSED(initial);
            NV_UNUSED(description);
            NV_UNUSED(saveMode);
            return nullptr;
        }

        TextureResult convertTexture(const char *sourceTexture,     // The fully qualified path name of the source texture
            const char *destTexture,       // The fully qualified path name of the dest texture
            const char *compressionFormat,// command line switches to pass to the nvcompress tool
            bool forceRebuildAllTextures)
        {
            NV_UNUSED(sourceTexture);
            NV_UNUSED(destTexture);
            NV_UNUSED(compressionFormat);
            NV_UNUSED(forceRebuildAllTextures);
            return TR_FAILED_NO_TOOL;
        }

        uint32_t convertTextures(const char *sourceTextures, const char *destTextures, const char *compressionFormat, bool forceRebuildAllTextures,TextureResult &result)
        {
            NV_UNUSED(sourceTextures);
            NV_UNUSED(destTextures);
            NV_UNUSED(forceRebuildAllTextures);
            result = TR_FAILED_NO_TOOL;
            return 0;
        }

#endif

} // end of lingcod namespace

