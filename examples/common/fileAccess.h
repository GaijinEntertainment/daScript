#pragma once
#ifdef _MSC_VER
    #include <io.h>
#else
    #include <dirent.h>
#endif

class FsFileInfo : public das::FileInfo {
public:
    virtual ~FsFileInfo() { 
        freeSourceData();
    }
private:
    virtual void freeSourceData() override {
        das_aligned_free16((void*)source);
        source = nullptr;
    }
};

class FsFileAccess : public das::FileAccess {
public:
    void verify() {
        for ( auto & fi : files ) {
            assert(fi.second.unique());
        }
    }
private:
    virtual das::FileInfoPtr getNewFileInfo(const das::string & fileName) override {
        if ( FILE * ff = fopen ( fileName.c_str(), "rb" ) ) {
            auto info = das::make_shared<FsFileInfo>();
            fseek(ff,0,SEEK_END);
            info->sourceLength = uint32_t(ftell(ff));
            fseek(ff,0,SEEK_SET);
            char *source = (char *) das_aligned_alloc16(info->sourceLength+1);
            fread(source, 1, info->sourceLength, ff);
            source[info->sourceLength] = 0;
            info->source = source;
            return setFileInfo(fileName, info);
        }
        return nullptr;
    }
};
