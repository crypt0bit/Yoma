#pragma once

int prepare_workspace(const char *path);
int extract_ipa(const char *ipa, const char *path, char *out_app);
int repack_ipa(const char *path, char *out_ipa);
