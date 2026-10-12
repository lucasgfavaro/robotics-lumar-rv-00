# 🔌 Proyecto IoT: [Nombre del Proyecto]

## 📝 Descripción
Breve resumen del proyecto.  
> Ejemplo: "Este proyecto usa un NodeMCU para medir temperatura con un sensor DHT11 y enviar los datos por Wi-Fi a un servidor MQTT."

---

## 🧰 Hardware

| Componente         | Modelo / Detalle             | Cantidad |
|--------------------|------------------------------|----------|
| Microcontrolador   | NodeMCU ESP8266              | 1        |
| Sensor             | [Ej. DHT11, LM35, etc.]      |          |
| Actuador (opcional)| [Ej. LED, relé, buzzer]      |          |
| Otros              | Protoboard, cables, USB      |          |

---

## 🔌 Conexiones

| Componente | Pin        | Descripción     |
|------------|------------|-----------------|
| Sensor     | D2         | Data            |
| LED        | D4 (GPIO2) | Salida digital  |

> 📷 Esquema visual opcional:  
> ![Esquema](img/esquema.png)

---

## 💻 Código

- Plataforma: ✅ Arduino IDE
- Lenguaje: C++

### Bibliotecas utilizadas
```cpp
#include <ESP8266WiFi.h>
#include <DHT.h>
