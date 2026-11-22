# Заметки

[![Discord](https://dcbadge.vercel.app/api/server/RP6ReXRn5j?style=flat)](https://discord.gg/RP6ReXRn5j)
[![Статус сборки GitHub Actions](https://github.com/nuttyartist/notes/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/nuttyartist/notes/actions/workflows/ci.yml?query=branch%3Amaster)

Notes — это приложение для заметок с открытым исходным кодом и поддержкой разных платформ, которое одновременно красиво и мощно.

Сайт: <a href="https://notes-foss.com" target="_blank">notes-foss.com</a>

## Скриншоты

https://github.com/nuttyartist/notes/assets/16375940/714ffe3e-95ef-4233-b3da-58fcea33fbfa

https://github.com/nuttyartist/notes/assets/16375940/370439b1-e42b-45c4-a807-0c3657051f97

## Обзор возможностей

- Нативное приложение (написано на C++ с использованием Qt).
- Превращайте задачи в Markdown в красивую доску Kanban.
- Быстрое и удобное.
- Полностью с открытым исходным кодом и кроссплатформенное (Linux, macOS, Windows).
- Полная приватность — ничего не отслеживает.
- Папки и теги. Организуйте идеи иерархически с помощью вложенных папок и универсально с помощью тегов.
- Поддержка Markdown. Форматируйте текст, не убирая руки с клавиатуры.
- Разные темы. Переключайтесь между светлой, тёмной и сепией.
- Режим ленты. Выбирайте несколько заметок, чтобы видеть их одну за другой в редакторе.
- Всегда работает в фоне. Используйте горячую клавишу <kbd>Win</kbd>+<kbd>Shift</kbd>+<kbd>N</kbd>, чтобы открыть Notes. <kbd>Ctrl</kbd>+<kbd>N</kbd> для macOS.
- Горячие клавиши. Приложение рассчитано на возможность полного использования с клавиатуры (но работа в этом направлении ещё продолжается).
- Какую функцию добавите вы?

## Горячие клавиши

Основные сочетания клавиш перечислены выше.

## Pro-версия

Чтобы поддержать разработку с открытым исходным кодом, вы можете приобрести [лицензию Pro-версии](https://notesapp.lemonsqueezy.com/checkout/buy/0e791d1e-9feb-4a67-b05a-f2f07f5c82ad?discount=0), которая позволит редактировать доску Kanban и получать бесплатные обновления Pro-версии в течение года.

Вы также можете поддержать проект пожертвованиями через [Github Sponsors](https://github.com/sponsors/nuttyartist) или [Patreon](https://www.patreon.com/rubymamis), либо назначить награду за решение задач через Bountysource.

При желании вы можете получить доступ ко всем Pro-функциям бесплатно, собрав приложение из исходного кода. См. раздел «Сборка из исходников» ниже.

## Сборка из исходного кода

Для сборки используйте корневой `CMakeLists.txt` и подходящий комплект Qt для вашей платформы.

## Путь к базе данных

База данных заметок и файл настроек хранятся в:

| ОС      | Путь                                                                                                                                                  |
| ------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| Windows | `%APPDATA%\Awesomeness`                                                                                                                               |
| Linux   | `~/.config/Awesomeness`<br>`~/snap/notes/current/.config/Awesomeness` (Snap)<br>`~/.var/app/io.github.nuttyartist.notes/config/Awesomeness` (Flatpak) |
| macOS   | `~/.config/Awesomeness`                                                                                                                               |

## Исходный проект

История разработки и сведения об участниках доступны в [репозитории Notes](https://github.com/nuttyartist/notes).

## Дорожная карта

Небольшой взгляд на то, что мы готовим дальше:

- Мобильное приложение на той же кодовой базе (адаптивное приложение).
- Замена базы данных с поддержкой произвольных папок.
- Встроенная синхронизация.
- Экспорт/импорт файлов .txt.
- Продвинутый редактор блоков.
- Браузерная версия на базе Qt для WebAssembly.
- И многое другое.

## Notes использует следующие сторонние библиотеки:

- [QMarkdownTextEdit](https://github.com/pbek/qmarkdowntextedit)
- [QSimpleUpdater](https://github.com/alex-spataru/QSimpleUpdater)
- [QAutostart](https://github.com/b00f/qautostart)
- [Qxt](https://bitbucket.org/libqxt/libqxt/src/master/)

## Notes использует следующие открытые шрифты:

- Roboto
- Source Sans Pro
- Trykker
- Mate
- iA Writer Mono
- iA Writer Duo
- iA Writer Quattro
- Font Awesome
- Material Symbols

[Условия использования и политика конфиденциальности](https://www.notes-foss.com/notes-app-terms-privacy-policy)

Подпись относится к текущей версии проекта.

---

Автор: legenda  
Telegram: [@LEGENDA_SD](https://t.me/LEGENDA_SD)
