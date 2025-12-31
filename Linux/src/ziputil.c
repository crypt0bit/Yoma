#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include "miniz.h"
#include "ziputil.h"

static void mkdirs(const char *p) {
    char tmp[1024];
    strcpy(tmp, p);
    for (char *s = tmp + 1; *s; s++)
        if (*s == '/') { *s = 0; mkdir(tmp, 0755); *s = '/'; }
}

int ipa_extract(const char *ipa, const char *out) {
    mz_zip_archive z = {0};
    if (!mz_zip_reader_init_file(&z, ipa, 0)) return -1;

    for (mz_uint i = 0; i < mz_zip_reader_get_num_files(&z); i++) {
        mz_zip_archive_file_stat st;
        mz_zip_reader_file_stat(&z, i, &st);
        mkdirs(st.m_filename);
        mz_zip_reader_extract_to_file(&z, i, st.m_filename, 0);
    }
    mz_zip_reader_end(&z);
    return 0;
}

static void add_dir(mz_zip_archive *z, const char *base, const char *path) {
    DIR *d = opendir(path);
    if (!d) return;

    struct dirent *e;
    char full[1024], rel[1024];

    while ((e = readdir(d))) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
        snprintf(full, sizeof(full), "%s/%s", path, e->d_name);
        snprintf(rel, sizeof(rel), "%s/%s", base, e->d_name);

        if (e->d_type == DT_DIR) add_dir(z, rel, full);
        else mz_zip_writer_add_file(z, rel, full, NULL, 0, MZ_BEST_COMPRESSION);
    }
    closedir(d);
}

int ipa_pack(const char *payload, const char *ipa) {
    mz_zip_archive z = {0};
    if (!mz_zip_writer_init_file(&z, ipa, 0)) return -1;
    add_dir(&z, payload, payload);
    mz_zip_writer_finalize_archive(&z);
    mz_zip_writer_end(&z);
    return 0;
}
