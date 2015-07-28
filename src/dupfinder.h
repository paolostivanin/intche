#ifndef DUPFINDER_H_INCLUDED
#define DUPFINDER_H_INCLUDED

#include <glib.h>

#define BUF_FILE 16777216
#define PROGRAM_NAME "DupFinder"
#define VERSION "0.99.1 (1.0-alpha1)"
#define DEVELOPER "Paolo Stivanin"
#define DEV_EMAIL "info@paolostivanin.com"
#define EXCLUDED_RELATIVE_PATH "/.config/dupfinder/excluded_folders"

goffset get_file_size (const gchar *);
gint is_dir (const gchar *);
gchar *compute_crc32 (const gchar *, goffset);
GSList *list_files_and_compute_chksum (const gchar *, GDir *);
gboolean is_folder_excluded (gchar *, gchar *);

#endif
