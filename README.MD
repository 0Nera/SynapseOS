# О проекте
<!-- -->

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white) ![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)

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

### Универсальное решение

```bash
python3 build.py
```

### [Linux Debian/Ubuntu]

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![Ubuntu](https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white) ![Debian](https://img.shields.io/badge/Debian-D70A53?style=for-the-badge&logo=debian&logoColor=white)

1. Установить ПО для сборки

    ``` bash
    sudo apt-get install python3 clang llvm grub-pc-bin xorriso mtools lld git fasm
    ```

2. Установить ПО для запуска ОС

    ``` bash
    sudo apt-get install libvirt-daemon libvirt-clients bridge-utils virt-manager qemu-kvm qemu virt-manager
    ```

    Или просто установите любую из поддерживаемых виртуальных машин

3. Скачиваем репозиторий с гитхаба (или самому через браузер)

    ```bash
    git clone https://github.com/0Nera/SynapseOS.git
    ```

4. Переходим в проект с папкой

    ```bash
    cd SynapseOS
    ```

5. Запускаем компиляцию

    Простая компиляция и запуск:
    ```bash
    python3 build.py
    ```

    Компиляция, без запуска:
    ```bash
    python3 build.py kernel apps iso
    ```

    Компиляция, без запуска, без приложений:
    ```bash
    python3 build.py kernel apps iso
    ```

### [Arch Linux]

1. Установить ПО для сборки
    ```bash
    sudo pacman -S python3 clang llvm grub xorriso mtools lld git fasm
    ```

2. Установить ПО для запуска ОС
    ```bash
    sudo pacman -S libvirt bridge-utils virt-manager qemu virt-manager
    ```

3. Скачиваем репозиторий с гитхаба (или самому через браузер)
    ```bash
    git clone https://github.com/0Nera/SynapseOS.git
    ```
4. Переходим в проект с папкой
    ```bash
    cd SynapseOS
    ```
5. Запускаем компиляцию
    Простая компиляция и запуск:
    ```bash
    python3 build.py
    ```

    Компиляция, без запуска:
    ```bash
    python3 build.py kernel apps iso
    ```

    Компиляция, без запуска, без приложений:
    ```bash
    python3 build.py kernel apps iso
    ```

### [Windows]

Установите WSL или виртуальную машину с Ubuntu версии не ниже 18.
Соберите ядро по инструкции для Linux.
В будущем, по мере перехода на Limine, сборка упростится в разы.

### [Другие ОС]

Когда-нибудь будет информация...

## Запуск в QEMU

Простой запуск:

```bash
qemu-system-i386 -name SynapseOS -cdrom SynapseOS.iso
```

С подключением устройств (Спикер, Сетевая карта RTL8139, откладочным портом в лог):

```bash
qemu-system-i386 -name SynapseOS -soundhw pcspk -m 16 -netdev socket,id=n0,listen=:2030 -device rtl8139,netdev=n0,mac=11:11:11:11:11:11 -cdrom SynapseOS.iso -serial file:Qemu.log
```

Стандартный:

```bash
python3 build.py run
```

## Минимальные системные требования

- 5 мегабайт оперативной памяти
- 4 мегабайта видеопамяти
- Процессор на x86 архитектуре

## Благодарности

- <https://vk.com/id_doczom>
- <https://github.com/rgimad>
- <https://vk.com/kernelmode_power>
- <https://vk.com/piminov_remont>
- <https://vk.com/ndraey>
- <https://vk.com/4auka228>
- Учитель

И другие

## Как внести свой вклад

### Если вы программист

- Сделайте форк репозитория GitHub;
- Создайте свою ветку если требуется
- Скачайте(склонируйте) репозиторий на своё устройство (опционально, вы можете редактировать файлы используя веб интерфейс GitHub)
- Внесите изменения
- (ВАЖНО) Протестируйте изменения
- Создайте pull request в этот репозиторий
- После проверки, ваши изменения скорее-всего примут, удачи!

Также не забывайте про стиль кода и коммитов, он указан в STYLE.md

### Если вы далеки от программирования

Вы можете протестировать ОС, придумать новую идею, задонатить на развитие или написать статью.

## Используемые ресурсы

- <https://wiki.osdev.org/Main_Page>
- <http://www.jamesmolloy.co.uk/tutorial_html/>
- <http://www.osdever.net/bkerndev/Docs/title.htm>
- <https://littleosbook.github.io>
- <http://www.brokenthorn.com/Resources/>
- <http://www.osdever.net/tutorials/>
- <https://github.com/rgimad/EOS>
- <https://phantomexos.blogspot.com/>

## Отказ от ответственности

SynapseOS это не дистрибутив linux, это новый проект который не имеет за собой компании или организации которая могла бы дать гарантий.
Ядро SynapseOS имеет открытый исходный код, вы можете сами удостовериться в отсутствии вредоносного ПО изучая файлы этого репозитория.
При использовании материалов вы обязуетесь соблюдать авторские права.
Я не несу ответственности за причиненный ущерб. Используйте на свой страх и риск.
