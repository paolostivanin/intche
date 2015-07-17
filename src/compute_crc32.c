#include <glib.h>
#include <glib/gstdio.h>
#include <gcrypt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "dupfinder.h"


gchar *compute_crc32 (const gchar *file_path, goffset file_size)
{
    GError *err = NULL;
    gint algo, i, fd, ret_val;
    gchar *crc32;
    guint8 *addr;
    const gchar *name = gcry_md_algo_name(GCRY_MD_CRC32);
    algo = gcry_md_map_name(name);
    goffset done_size = 0, diff = 0, offset = 0;
    
    fd = g_open (file_path, O_RDONLY | O_NOFOLLOW);
    if (fd == -1)
    {
        g_printerr ("compute_crc32: %s\n", g_strerror (errno));
        return NULL;
    }
    
    gcry_md_hd_t hd;
    gcry_md_open(&hd, algo, 0);

    if (file_size < BUF_FILE)
    {
        addr = mmap (NULL, file_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED)
        {
            g_printerr ("compute_crc32: %s\n", g_strerror (errno));
            return NULL;
        }
        gcry_md_write (hd, addr, file_size);
        ret_val = munmap (addr, file_size);
        if (ret_val == -1)
        {
            g_printerr ("compute_crc32: %s\n", g_strerror (errno));
            return NULL;
        }
    }
    else
    {
       while (file_size > done_size)
       {
           addr = mmap (NULL, BUF_FILE, PROT_READ, MAP_FILE | MAP_SHARED, fd, offset);
           if (addr == MAP_FAILED)
           {
               g_printerr ("compute_crc32: %s\n", g_strerror (errno));
               return NULL;
           }
           gcry_md_write (hd, addr, BUF_FILE);
           done_size += BUF_FILE;
           diff = file_size - done_size;
           offset += BUF_FILE;
           if (diff < BUF_FILE && diff > 0)
           {
               addr = mmap (NULL, diff, PROT_READ, MAP_FILE | MAP_SHARED, fd, offset);
               if (addr == MAP_FAILED)
               {
                   g_printerr ("compute_crc32: %s\n", g_strerror (errno));
                   return NULL;
               }
               gcry_md_write (hd, addr, diff);
               ret_val = munmap (addr, diff);
               if (ret_val == -1)
               {
                   g_printerr ("compute_crc32: %s\n", g_strerror (errno));
                   return NULL;
               }
               break;
           }
           ret_val = munmap (addr, BUF_FILE);
           if (ret_val == -1)
           {
               g_printerr ("compute_crc32: %s\n", g_strerror (errno));
               return NULL;
           }
       }
    }
    gcry_md_final (hd);
    crc32 = g_malloc (9);
    guchar *cksum = gcry_md_read (hd, algo);
    for (i=0; i<4; i++)
        g_sprintf (crc32+(i*2), "%02x", cksum[i]);
    crc32[8] = '\0';
    gcry_md_close (hd);
    g_close (fd, &err);

    return crc32;
}
