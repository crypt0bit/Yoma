#import "ZipUtil.h"
#import "miniz.h"
#include <sys/stat.h>
#include <dirent.h>

@implementation ZipUtil

+ (void)mkdirs:(NSString *)path {
    NSArray *components = [path pathComponents];
    NSString *p = @"";
    for (NSString *c in components) {
        p = [p stringByAppendingPathComponent:c];
        [[NSFileManager defaultManager] createDirectoryAtPath:p
                                  withIntermediateDirectories:YES
                                                   attributes:nil
                                                        error:nil];
    }
}

+ (BOOL)extractIPA:(NSString *)ipaPath to:(NSString *)outputPath {
    mz_zip_archive zip = {0};
    if (!mz_zip_reader_init_file(&zip, [ipaPath UTF8String], 0)) return NO;

    for (mz_uint i = 0; i < mz_zip_reader_get_num_files(&zip); i++) {
        mz_zip_archive_file_stat st;
        mz_zip_reader_file_stat(&zip, i, &st);
        [self mkdirs:[NSString stringWithUTF8String:st.m_filename]];
        mz_zip_reader_extract_to_file(&zip, i, st.m_filename, 0);
    }

    mz_zip_reader_end(&zip);
    return YES;
}

+ (void)addDir:(mz_zip_archive *)zip base:(NSString *)base path:(NSString *)path {
    DIR *d = opendir([path UTF8String]);
    if (!d) return;

    struct dirent *e;
    char full[1024], rel[1024];

    while ((e = readdir(d))) {
        if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;

        snprintf(full, sizeof(full), "%s/%s", [path UTF8String], e->d_name);
        snprintf(rel, sizeof(rel), "%s/%s", [base UTF8String], e->d_name);

        if (e->d_type == DT_DIR)
            [self addDir:zip base:[NSString stringWithUTF8String:rel] path:[NSString stringWithUTF8String:full]];
        else
            mz_zip_writer_add_file(zip, rel, full, NULL, 0, MZ_BEST_COMPRESSION);
    }
    closedir(d);
}

+ (BOOL)packPayload:(NSString *)payloadPath toIPA:(NSString *)ipaPath {
    mz_zip_archive zip = {0};
    if (!mz_zip_writer_init_file(&zip, [ipaPath UTF8String], 0)) return NO;
    [self addDir:&zip base:payloadPath path:payloadPath];
    mz_zip_writer_finalize_archive(&zip);
    mz_zip_writer_end(&zip);
    return YES;
}

@end
