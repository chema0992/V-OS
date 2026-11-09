#!/bin/bash
set -e

echo "=========================================="
echo "       Building V-OS (ㅸ-OS) ISO          "
echo "=========================================="

# 1. rootfs 구조 세팅 및 기존 rootfs/init 디렉터리 정리
echo "[1/4] V-OS C Shell 컴파일 (정적 라이브러리 링크)..."
mkdir -p rootfs/bin rootfs/dev rootfs/proc rootfs/sys rootfs/etc

# rootfs/init이 디렉터리로 존재하는 경우 삭제하여 gcc 충돌 방지
if [ -d "rootfs/init" ]; then
    rm -rf rootfs/init
fi

# 1) PID 1: 최소 init (마운트/스플래시 + 쉘 재실행)
gcc -static \
    src/init/init.c \
    src/shell/reset.c \
    -o rootfs/init
chmod +x rootfs/init

# 2) 일반 쉘
gcc -static \
    src/shell/main.c \
    src/shell/shell.c \
    src/shell/env.c \
    src/shell/commands.c \
    src/shell/commands/*.c \
    -o rootfs/bin/vsh
chmod +x rootfs/bin/vsh

# 3) 실행 파일 복사
cp app/no rootfs/bin/no
chmod +x rootfs/bin/no
cp app/nos rootfs/bin/nos
chmod +x rootfs/bin/nos

# 2. initramfs 램디스크 압축
echo "[2/4] rootfs -> initramfs.cpio.gz 램디스크 압축..."
mkdir -p iso_root/boot/grub

cd rootfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../iso_root/boot/initramfs.cpio.gz
cd ..

# 3. GRUB2 설정 및 커널 바이너리 스테이징
echo "[3/4] GRUB2 설정 및 커널 바이너리 복사..."
cp configs/grub.cfg iso_root/boot/grub/grub.cfg

# 커널 bzImage 경로 지정 (실제 커널 위치)
KERNEL_PATH="kernel/linux-6.18.55/arch/x86/boot/bzImage"

if [ -f "$KERNEL_PATH" ]; then
    cp "$KERNEL_PATH" iso_root/boot/bzImage
    echo " -> 커널 복사 완료 ($KERNEL_PATH)"
else
    # 지정 경로에 없을 경우 자동 검색
    FOUND_KERNEL=$(find kernel/ -name bzImage 2>/dev/null | head -n 1)
    if [ -n "$FOUND_KERNEL" ]; then
        cp "$FOUND_KERNEL" iso_root/boot/bzImage
        echo " -> 커널 감지 및 복사 완료 ($FOUND_KERNEL)"
    else
        echo "ERROR: bzImage 커널 파일을 찾을 수 없습니다."
        exit 1
    fi
fi

# 4. 최종 ISO 빌드
echo "[4/4] 최종 build/v-os.iso 생성..."
mkdir -p build
grub-mkrescue -o build/v-os.iso iso_root

echo "=========================================="
echo " 🎉 SUCCESS: build/v-os.iso 생성 완료!   "
echo "=========================================="
