# CompAdmiss
Модуль управления системным блоком компьютера с помощью компьютера или телефона.

- **Данный модуль имеет несколько функций:**
    - Управление включением и выключением компьютера.
    - Управление подcветкой системного блока.
    - Управление вентиляторами системного блока.
    - Вывод всей информации на LCD экран.

- **Весь процесс основан на выборе режима работы. Имеются несколько режимов:**
    - Спокойный, вентиляторы либо включены на минимум, либо выключены вообще.
    - Средний или эко, вентиляторы работают на средней мощности.
    - Производительный, вентиляторы работают на максимум.
    - Адский, вентиляторы работают на 200%, подцветка становится крассно(плюс 100 к скиллу).
     делается все, чтобы компьютер не взорвался.
***


- **Управление включением и выключением компьютера:**
    - Реализованно с помощью одного провода, на который подается 5V.
    - Управление происходит при помощи телефона.

- **Управление подcветкой системного блока:**
    - Использованны N канальные транзисторы IRF3205.
    - Управление происходит при помощи приложения OpenHardwareMonitor или телефона.

- **Управление вентиляторами системного блока:**
    - Использованны N канальные транзисторы IRF3205.
    - Управление происходит при помощи приложения OpenHardwareMonitor или телефона.
***


- **Для разработчиков:**
    - **Спокойный режим:**
        - **Вентиляторы:**
            - Вентиляторы выключены, но при этом мы всеравно следим за температурой процессора.
            - Если температура выше нормы(55'), то увеличиваем скорость веляторов на 20%.
            - Если температура ниже нормы(55'), то вентиляторы выключены.
        - **Подcветка**
            - Светит спокойный цвет, по выбору пользователя.
            - Если температура увеличивается, то гамма тоже увеличивется.
    - **Средний режим**
        - **Вентиляторы:**
            - Вентиляторы работают на 50%, но следим на температурой.
            - Если температура выше нормы(70'), то увеличим скорость вентиляторов до 80%.
            - Если температура ниже нормы(70'), то вентиляторы возращаются к 50%.
        - **Подcветка**
            - Светят яркие цвета, по выбору пользователя.
            - Если температура увеличивается, то гамма тоже увеличивется.
    - **Производительный режим**
        - **Вентиляторы:**
            - Вентиляторы работают на 80%, но следим на температурой.
            - Если температура выше нормы(80'), то увеличим скорость вентиляторов до 100%.
            - Если температура ниже нормы(80'), то вентиляторы возращаются к 80%.
        - **Подcветка**
            - Светят цвета близкие к крассному оттенку, по выбору пользователя.
            - Если температура увеличивается, то гамма тоже увеличивется.
    - **Адский режим**
        - **Вентиляторы:**
            - Вентиляторы работают на 100%, но следим на температурой.
        - **Подcветка**
            - Светит только крассный цвет.
            - Если температура увеличивается, то подцветка начинает мигать как палицейская мигалка.
        


