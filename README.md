# ㅸ OS (V-OS)
리눅스 커널 기반의 OS입니다.
쉘(Vsh)을 직접 만드는 중이예요.

(개발 과정에서 AI를 사용했습니다)

## 🛠️ 빌드 전 사전 준비 (Prerequisites)

이 프로젝트는 용량 관계상 리눅스 커널 소스 및 바이너리를 포함하고 있지 않습니다.

### 1. 커널 바이너리 준비 및 배치
* **Linux Kernel 6.18.55** (또는 호환 버전)를 빌드하여 생성된 커널 이미지(`bzImage`)가 필요합니다.
* 프로젝트 루트에 `kernel/` 디렉터리를 만들고, 빌드가 완료된 커널 소스 폴더 전체를 통째로 넣어주거나 혹은 `bzImage` 파일만 구조에 맞게 배치해 주세요.
  * *정석 경로:* `kernel/linux-6.18.55/arch/x86/boot/bzImage`
  * (스크립트 내부의 `find kernel/ -name bzImage` 로직이 `kernel/` 하위 폴더를 자동으로 탐색하므로, 빌드된 커널 소스 트리가 통째로 `kernel/` 밑에 들어가 있어도 알아서 잘 찾아냅니다.)

### 2. 필수 빌드 도구 설치
`build.sh`를 실행하여 ISO를 만드려면 시스템에 아래 패키지들이 설치되어 있어야 합니다.

```bash
sudo apt-get update
sudo apt-get install -y build-essential cpio xorriso grub-pc-bin
```

### 3. 빌드
` ./build.sh ` (GCC가 필요합니다.)

그 후, QEMU 등으로 iso 파일을 에뮬레이션 하시면 됩니다.
