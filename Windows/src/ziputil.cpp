#include "ziputil.h"
#include "miniz.h"
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <cstring>
#include <iostream>

static void mkdirs(const std::string &path) {
    std::string tmp;
    for (char c : path) {
        tmp += c;
        if (c == '\\' || c == '/') {
            _mkdir(tmp.c_str());
        }
    }
}

bool ZipUtil::extractIPA(const std::string &ipaPath, const std::string &outputPath) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, ipaPath.c_str(), 0)) return false;

    for (mz_uint i = 0; i < mz_zip_reader_get_num_files(&zip); i++) {
        mz_zip_archive_file_stat st;
        mz_zip_reader_file_stat(&zip, i, &st);
        mkdirs(st.m_filename);
        mz_zip_reader_extract_to_file(&zip, i, st.m_filename, 0);
    }
    mz_zip_reader_end(&zip);
    return true;
}

static void addDir(mz_zip_archive *zip, const std::string &base, const std::string &path) {
    DIR *d = opendir(path.c_str());
    if (!d) return;

    struct dirent *e;
    char full[1024], rel[1024];

    while ((e = readdir(d))) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        snprintf(full, sizeof(full), "%s/%s", path.c_str(), e->d_name);
        snprintf(rel, sizeof(rel), "%s/%s", base.c_str(), e->d_name);

        if (e->d_type == DT_DIR) addDir(zip, rel, full);
        else mz_zip_writer_add_file(zip, rel, full, NULL, 0, MZ_BEST_COMPRESSION);
    }
    closedir(d);
}

bool ZipUtil::packPayload(const std::string &payloadPath, const std::string &ipaPath) {
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_writer_init_file(&zip, ipaPath.c_str(), 0)) return false;
    addDir(&zip, payloadPath, payloadPath);
    mz_zip_writer_finalize_archive(&zip);
    mz_zip_writer_end(&zip);
    return true;
}
