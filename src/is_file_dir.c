#include <glib.h>
#include <glib/gstdio.h>
#include <gio/gio.h>

gint
is_dir (const gchar *file_path)
{
	GFileInfo *info;
	GFile *file;
	GError *error = NULL;
    gint ret_val;

	const gchar *attributes = "standard::*";
	GFileQueryInfoFlags flags = G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS;
	GCancellable *cancellable = NULL;
    GFileType file_type;

	file = g_file_new_for_path (file_path);
    info = g_file_query_info (G_FILE(file), attributes, flags, cancellable, &error);
    if (error != NULL)
    {
        g_print ("%s\n", error->message);
        return -1;
    }
    file_type = g_file_info_get_file_type (info);

    if (file_type == G_FILE_TYPE_DIRECTORY)
    {
        if (file_type != G_FILE_TYPE_SYMBOLIC_LINK)
            ret_val = 0;
        else
            ret_val = -1;
    }
    else
    {
        if (file_type == G_FILE_TYPE_REGULAR && file_type != G_FILE_TYPE_SYMBOLIC_LINK)
            ret_val = 1;
        else
            ret_val = -1;
    }
	g_object_unref (file);
	
	return ret_val;
}
