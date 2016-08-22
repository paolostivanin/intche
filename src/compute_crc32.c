#include <glib.h>
#include <glib/gstdio.h>
#include <gcrypt.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "dupfinder.h"

#define CRC32_DIGEST_SIZE 4


gchar *compute_crc32 (const gchar *file_path, goffset file_size)
{
    GError *err = NULL;
    const gchar *name = gcry_md_algo_name(GCRY_MD_CRC32);
    gint algo = gcry_md_map_name(name);
    goffset done_size = 0, diff = 0, offset = 0;
    
    gint fd = g_open (file_path, O_RDONLY | O_NOFOLLOW);
    if (fd == -1) {
        g_printerr ("compute_crc32: %s\n", g_strerror (errno));
        return NULL;
    }
    
    gcry_md_hd_t hd;
    gcry_md_open(&hd, algo, 0);

    guint8 *addr;
    gint ret_val;
    if (file_size < BUF_FILE) {
        addr = mmap (NULL, file_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
        if (addr == MAP_FAILED) {
            g_printerr ("compute_crc32: %s\n", g_strerror (errno));
            return NULL;
        }
        gcry_md_write (hd, addr, file_size);
        ret_val = munmap (addr, file_size);
        if (ret_val == -1) {
            g_printerr ("compute_crc32: %s\n", g_strerror (errno));
            return NULL;
        }
    }
    else {
       while (file_size > done_size) {
           addr = mmap (NULL, BUF_FILE, PROT_READ, MAP_FILE | MAP_SHARED, fd, offset);
           if (addr == MAP_FAILED) {
               g_printerr ("compute_crc32: %s\n", g_strerror (errno));
               return NULL;
           }
           gcry_md_write (hd, addr, BUF_FILE);
           done_size += BUF_FILE;
           diff = file_size - done_size;
           offset += BUF_FILE;
           if (diff < BUF_FILE && diff > 0) {
               addr = mmap (NULL, diff, PROT_READ, MAP_FILE | MAP_SHARED, fd, offset);
               if (addr == MAP_FAILED) {
                   g_printerr ("compute_crc32: %s\n", g_strerror (errno));
                   return NULL;
               }
               gcry_md_write (hd, addr, diff);
               ret_val = munmap (addr, diff);
               if (ret_val == -1) {
                   g_printerr ("compute_crc32: %s\n", g_strerror (errno));
                   return NULL;
               }
               break;
           }
           ret_val = munmap (addr, BUF_FILE);
           if (ret_val == -1) {
               g_printerr ("compute_crc32: %s\n", g_strerror (errno));
               return NULL;
           }
       }
    }
    
    gcry_md_final (hd);
    
    gchar *crc32 = g_malloc (CRC32_DIGEST_SIZE * 2);
    guchar *cksum = gcry_md_read (hd, algo);
    
    for (gint i = 0; i < CRC32_DIGEST_SIZE; i++) {
        g_sprintf (crc32+(i*2), "%02x", cksum[i]);
    }
    crc32[CRC32_DIGEST_SIZE * 2] = '\0';
    
    gcry_md_close (hd);
    g_close (fd, &err);

    return crc32;
}
