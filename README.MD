# О проекте
<!-- -->

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white) ![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54) ![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)


SynapseOS - простая x86 операционная система на языке C с хорошо документированным ядром.
![SynapseOS](https://raw.githubusercontent.com/0Nera/SynapseOS/master/screenshots/test.png "SynapseOS")

## Имеется

- PS/2 клавиатура
- Kheap
- VMM
- PMM
- VFS
- ELF 32 формат исполняемых файлов
- VESA графика
- CMOS
- PC Speaker
- PCI
- Поддержка ATA
- VFS
- tar-fs
- SSFS- безопасная файловая система
- 11 сисфункций

## Сборка и запуск

### [Linux Debian/Ubuntu]

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white) ![Debian](https://img.shields.io/badge/Debian-D70A53?style=for-the-badge&logo=debian&logoColor=white)

1. Установить ПО для сборки

    `sudo apt-get install python3 clang llvm grub-pc-bin xorriso mtools  lld git`

2. Установить ПО для запуска ОС

    `sudo apt-get install libvirt-daemon libvirt-clients bridge-utils virt-manager qemu-kvm qemu virt-manager`

3. Скачиваем репозиторий с гитхаба (или самому через браузер)

    `git clone https://github.com/0Nera/SynapseOS.git`

4. Переходим в проект с папкой

    `cd SynapseOS`

5. Запускаем компиляцию

    Простая компиляция и запуск - `python3 build.py`

    Компиляция, без запуска - `python3 build.py kernel apps iso`

    Компиляция, без запуска, без приложений - `python3 build.py kernel apps iso`

### [Windows]

Установите WSL или виртуальную машину с Ubuntu не ниже 18.
Соберите ядро по инструкции для Linux.
В будущем, по мере перехода на Limine, сборка упростится в разы.

### [Другие ОС]

Когда-нибудь будет информация...

## Запуск в QEMU

Простой запуск:

`qemu-system-i386 -name SynapseOS -cdrom SynapseOS.iso`

С подключением устройств (Спикер, Сетевая карта RTL8139, откладочным портом в лог):

`qemu-system-i386 -name SynapseOS -soundhw pcspk -m 16 -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 -cdrom SynapseOS.iso -serial file:Qemu.log`

Стандартный:

`python3 build.py run`

## Минимальные системные требования

- 5 мегабайт оперативной памяти
- 4 мегабайта видеопамяти
- Процессор на x86 архитектуре

## Благодарности

- <https://vk.com/id_doczom>
- <https://github.com/rgimad>

## Используемые ресурсы

- <https://wiki.osdev.org/Main_Page>
- <http://www.jamesmolloy.co.uk/tutorial_html/>
- <http://www.osdever.net/bkerndev/Docs/title.htm>
- <https://littleosbook.github.io>
- <http://www.brokenthorn.com/Resources/>
- <http://www.osdever.net/tutorials/>
- <https://github.com/rgimad/EOS>
- <https://phantomexos.blogspot.com/>
