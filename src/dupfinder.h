#ifndef DUPFINDER_H_INCLUDED
#define DUPFINDER_H_INCLUDED

#include <glib.h>

#define BUF_FILE 16777216

goffset get_file_size (const gchar *);
gint is_dir (const gchar *);
gchar *compute_crc32 (const gchar *, goffset);

#endif
