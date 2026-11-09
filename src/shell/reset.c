#include "reset.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <string.h>
#include <errno.h>

static int fb_fd = -1;
static char *fbp = MAP_FAILED;
static long screensize = 0;

static void show_splash(void) {
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd < 0) {
        return;
    }

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) < 0 ||
        ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        close(fb_fd);
        fb_fd = -1;
        return;
    }

    screensize = finfo.smem_len;
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (fbp == MAP_FAILED) {
        close(fb_fd);
        fb_fd = -1;
        return;
    }

    int img_fd = open("/etc/splash.raw", O_RDONLY);
    if (img_fd < 0) {
        memset(fbp, 0, screensize);
        return;
    }

    int bpp = vinfo.bits_per_pixel / 8;
    if (bpp == 0) bpp = 4;

    int img_w = 1280;
    int img_h = 720;
    size_t img_size = (size_t)img_w * img_h * bpp;

    char *img_buf = malloc(img_size);
    if (!img_buf) {
        close(img_fd);
        return;
    }

    ssize_t read_bytes = read(img_fd, img_buf, img_size);
    close(img_fd);

    if (read_bytes <= 0) {
        free(img_buf);
        memset(fbp, 0, screensize);
        return;
    }

    int copy_w = img_w;
    int copy_h = img_h;

    if (copy_w > (int)vinfo.xres) copy_w = vinfo.xres;
    if (copy_h > (int)vinfo.yres) copy_h = vinfo.yres;

    for (int y = 0; y < copy_h; y++) {
        long fb_offset  = (long)y * finfo.line_length;
        long img_offset = (long)y * img_w * bpp;
        memcpy(fbp + fb_offset, img_buf + img_offset, (size_t)copy_w * bpp);
    }

    free(img_buf);
}

static void hide_splash(void) {
    if (fbp != MAP_FAILED) {
        memset(fbp, 0, screensize);
        munmap(fbp, screensize);
        fbp = MAP_FAILED;
    }
    if (fb_fd >= 0) {
        close(fb_fd);
        fb_fd = -1;
    }
}

void reset_system(void) {
    mount("devtmpfs", "/dev", "devtmpfs", 0, NULL);
    mount("proc",     "/proc", "proc",     0, NULL);
    mount("sysfs",    "/sys",  "sysfs",    0, NULL);

    show_splash();
    sleep(3);
    hide_splash();
}
