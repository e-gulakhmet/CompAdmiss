# CompAdmiss
Модуль управления системным блоком компьютера с помощью компьютера или телефона.

- **Данный модуль имеет несколько функций:**
    - Управление включением и выключением компьютера.
    - Управление подcветкой системного блока.
    - Управление вентиляторами системного блока.
    - Слежение за температурой и нагрузкой компьютера.
    - Оповещение о высокой температуре.
***


- **Управление включением и выключением компьютера:**
    - Реализованно с помощью одного провода, на который подается 5V.
    - Управление происходит при помощи пульта.

- **Управление подcветкой системного блока:**
    - Использованны N канальные транзисторы IRF3205.
    - Управление происходит при помощи приложения CompAdmissApp или пульта.

- **Управление вентиляторами системного блока:**
    - Использованны N канальные транзисторы IRF3205.
    - Управление происходит при помощи приложения CompAdmissApp или пульта.
***


- **Работа вентиляторов**
    - Зависит от температуры процессора и видеокарты.
    - Имеется два режима **On** или **Off**. 
    - Если температура выше среднего значения, то вентиляторы включаеются.
    - Если температура меньше, то вентиляторы выключаются.
    - Вентиляторами можно управлять вручную. Если вентиляторы выключены,
        а температура высокая, то они сами вкючатся.

- **Работа подсветки**
    - Сдедит за температурой и нагрузкой видеокарты и процессора.
    - При запуске включается выбранный режим подсветки, затем взависимости от режима работы
        цвета становятся более яркими(теплые цвета перестают использоваться).
    - Цвета ленты на прямую зависят от температуры, так как используется "map"

    - Первый и основной режим подсветки - это изменение цвета взависимости от температуры.
    - Второй режим подсветки - это радуга которая при повышении температуры,
        уходит в белые тона.
    - Третий режим - измение оттенка красного цвета, при повышении температуры.
    

- **Содержимое массива PCdata**
    0. CPU temp,
    1. GPU temp,
    2. mother temp,
    3. max HDD temp,
    4. CPU load,
    5. GPU load,
    6. RAM use,
    7. GPU memory use,
    8. maxFAN,
    9. minFAN,
    10. maxTEMP,
    11. minTEMP,
    12. manualFAN,
    13. manualCOLOR,
    14. fanCtrl,
    15. colorCtrl,
    16. brightCtrl,
    17. LOGinterval,
    18. tempSource
    


