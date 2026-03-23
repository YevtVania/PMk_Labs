# Лабораторна робота №2: Використання UART для виведення інформації

## Курс: Програмування мікроконтролерів (ПМК) | УжНУ

Цей репозиторій містить проєкт для середовища **PSoC Creator 4.4**, спрямований на вивчення роботи інтерфейсу UART. Програма передає текстові повідомлення про стан кнопки через UART на комп'ютер та керує RGB-світлодіодом залежно від стану кнопки.

---

## 📋 Суть роботи

- Налаштування компонента **Software Transmit UART** (115200 бод)
- Керування **RGB-світлодіодом** (два режими кольору)
- Зчитування стану кнопки через `Button_Read()`
- Виведення повідомлень у термінал:
  - При затисканні кнопки → **жовтий** колір + `"Ya vykonuyu laboratornu robotu #2"`
  - При відпуску кнопки → **фіолетовий** колір + `"Knopka OFF"`
- Блимання LED з частотою **2 Гц** (250 мс ON / 250 мс OFF)

---

## 🎨 Логіка кольорів RGB

> PSoC Pioneer Kit використовує **активний LOW** — `0` = увімкнено, `1` = вимкнено

| Стан кнопки | Колір LED | RED | GREEN | BLUE |
|---|---|---|---|---|
| Затиснута (Button == 0) | 🟡 Жовтий (Yellow) | 0 | 0 | 1 |
| Відпущена (Button == 1) | 🟣 Фіолетовий (Purple) | 0 | 1 | 0 |

### Повна таблиця RGB (3 біти)

| Колір | RED | GREEN | BLUE |
|---|---|---|---|
| White | 0 | 0 | 0 |
| Yellow | 0 | 0 | 1 |
| Purple | 0 | 1 | 0 |
| Red | 0 | 1 | 1 |
| Cyan | 1 | 0 | 0 |
| Green | 1 | 0 | 1 |
| Blue | 1 | 1 | 0 |
| Black | 1 | 1 | 1 |

---

## 🛠 Необхідне ПЗ

- **PSoC Creator 4.4**
- **PSoC Programmer**
- Термінал (будь-який на вибір):
  - [PuTTY](https://www.putty.org)
  - [TeraTerm](https://teratermproject.github.io/index-en.html)

---

## ⚙️ Конфігурація пінів

| Компонент | Порт | Пін |
|---|---|---|
| SW_Tx_UART:tx | P2[7] | 9 |
| Button | P0[7] | 31 |
| LED (RED) | P1[6] | 43 |
| LED2 (GREEN) | P0[2] | 26 |
| LED3 (BLUE) | P0[3] | 27 |

---

## 🚀 Запуск

### Клонування
```bash
git clone https://github.com/ERRORES241/PMk_Lab2_UzhNU.git
```

### Відкриття
Відкрити `.cywrk` у **PSoC Creator 4.4**

### Оновлення компонентів
Погодитися на **Update Components**, якщо запропоновано

### Налаштування перемикачів плати (SW4)

| Перемикач | Положення |
|---|---|
| SW4.1 | OFF |
| SW4.2 | OFF |
| SW4.3 | ON |

> SW4.3 = ON підключає вивід PSoC до входу програматора (Software TX → P2[7])

### Збірка і прошивка

| Дія | Команда |
|---|---|
| Compile | `Ctrl + F6` |
| Build | `Shift + F6` |
| Program | `Ctrl + F5` |

### Підключення терміналу

- **COM-порт:** KitProg USB-UART (переглянути в Диспетчері пристроїв)
- **Швидкість:** `115200` бод
- **Data bits:** 8
- **Parity:** None
- **Stop bits:** 1
- **Flow control:** None

Після натискання **Reset** на платі — у терміналі з'являться повідомлення.

---

## 💻 Фрагмент коду (`main.c`)

```c
#include "project.h"

int main(void)
{
    CyGlobalIntEnable;
    SW_Tx_UART_Start();

    for(;;)
    {
        // Кнопку ЗАТИСНУТО -> ЖОВТИЙ колір
        if(Button_Read() == 0)
        {
            SW_Tx_UART_PutString("Ya vykonuyu laboratornu robotu #2\r\n");

            LED_Write(0);    // RED = вимк
            LED2_Write(0);   // GREEN = вимк
            LED3_Write(1);   // BLUE = увімк -> Yellow

            CyDelay(250);
            LED_Write(1); LED2_Write(1); LED3_Write(1);
            CyDelay(250);
        }
        // Кнопка ВІДПУЩЕНА -> ФІОЛЕТОВИЙ колір
        else
        {
            SW_Tx_UART_PutString("Knopka OFF\r\n");

            LED_Write(0);    // RED = вимк
            LED2_Write(1);   // GREEN = увімк
            LED3_Write(0);   // BLUE = вимк -> Purple

            CyDelay(250);
            LED_Write(1); LED2_Write(1); LED3_Write(1);
            CyDelay(250);
        }
    }
}
```

---

## 📂 Структура файлів

```
PMk_Lab2_UzhNU/
├── TopDesign.cysch     — схема проєкту (UART + Button + RGB LED)
├── main.c              — основна логіка програми
├── *.cydwr             — конфігурація пінів мікроконтролера
└── README.md           — опис проєкту
```

---

## 📸 Результат

| Подія | Термінал | LED |
|---|---|---|
| Reset плати | `Software Transmit UART` | — |
| Кнопку затиснуто | `Ya vykonuyu laboratornu robotu #2` | 🟡 Жовтий (блимає) |
| Кнопку відпущено | `Knopka OFF` | 🟣 Фіолетовий (блимає) |

---

## 👤 Автор

**Євтушик Іван** — студент 2-го курсу, спеціальність 123 «Комп'ютерна інженерія»  
Кафедра комп'ютерних систем та мереж, УжНУ, 2026
