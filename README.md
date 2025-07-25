WebRTC Chat Application (Qt + WebEngine + HTML + Signaling Server)
Приложение для аудио- и видеозвонков на базе WebRTC, встраиваемое в интерфейс Qt с помощью QWebEngineView. Для установления соединения используется WebSocket-сервер сигналинга на Python.


Требования:
1. Qt:
- Версия: Qt 5.15+ или Qt 6+
- Модуль: QtWebEngineWidgets (обязательно)
- Компилятор:
  - Windows: MSVC (предпочтительно) или MinGW
  - Linux: GCC

2. Python:
- Версия: Python 3.8+
- Зависимость: websockets


Установка и сборка:
Windows:
1. Убедитесь, что установлен Qt с модулем Qt WebEngine Widgets.
2. Установите Python и зависимость:
   pip install websockets
3. Сборка через CMake:
   mkdir build
   cd build
   cmake ..
   cmake --build .
4. Запуск:
   python ../signaling_server.py
   ./WebRTCChatApp
   
Linux:
1. Установите Qt и WebEngine:
   sudo apt install qt6-base-dev qt6-webengine-dev
2. Установите Python и зависимость:
   sudo apt install python3-pip
   pip install websockets
3. Сборка:
   mkdir build
   cd build
   cmake ..
   make
4. Запуск:
   python3 ../signaling_server.py
   ./WebRTCChatApp


Работа приложения:
1. HTML-файл (`webrtc.html`) реализует клиент WebRTC + WebSocket.
2. Сервер сигналинга (`signaling_server.py`) обеспечивает обмен offer/answer/ICE-кандидатами.
3. Qt-приложение отображает `webrtc.html` через `QWebEngineView`.


Планы на будущее:
- Добавить поддержку комнаты/идентификатора пользователя
- Хранение истории чата
- Возможность подключения к signaling-серверу не только локально