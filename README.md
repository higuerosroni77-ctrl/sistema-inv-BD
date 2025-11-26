#  Sistema CRUD de Inventario y Ventas – Importadora RCE  
### Aplicación en C++ con base de datos SQLite

Este proyecto es un sistema completo de **Inventario + Control de Ventas** desarrollado en **C++** y utilizando **SQLite** como motor de base de datos.  
Permite registrar productos, gestionar inventario, almacenar ventas por mes y generar reportes anuales detallados, todo desde una interfaz de consola con colores ANSI.

---

##  Características principales

### 🛒 **Gestión de inventario (CRUD)**
- Registrar nuevos productos
- Mostrar listado completo
- Actualizar datos (nombre, cantidad, precio)
- Eliminar productos

###  **Módulo de ventas**
- Registrar ventas por mes (1–12)
- Verificación previa del producto
- Consulta de ventas registradas
- Total de ventas por producto

###  **Consultas avanzadas**
- Productos con bajo stock (límite configurable)
- Totales de ventas agrupadas
- Reporte anual de ventas con desglose por mes
- Suma total anual y por producto

###  **Interfaz de consola mejorada**
- Colores ANSI
- Tablas alineadas con `iomanip`
- Limpieza y pausa automática con `system("cls")` y `system("pause")`

---

##  Estructura del proyecto

- **main.cpp** — Lógica completa del programa (menús, CRUD, reportes)  
- **sqlite3.c / sqlite3.h** — Motor de base de datos SQLite integrado  
- **inventario.db** — Base de datos generada automáticamente  
- **CRUD Importadora.zpr** — Archivo del proyecto para Zinjai  

> El código fuente principal proviene del archivo:  
> :contentReference[oaicite:1]{index=1}

---

##  Requisitos

###  Compilador soportado
- MinGW (GNU G++)
- Zinjai (recomendado)
- CodeBlocks  
- Dev-C++ (FUNCIONA, pero requiere incluir `sqlite3.c` dentro del proyecto)

###  Librerías incluidas
- SQLite3 (incluida en el proyecto, no necesita instalación)
- iostream, string, iomanip
- map, array  
- Colores ANSI en consola  
- `system("cls")` y `system("pause")` (requiere Windows)

---

##  Ejecución

1. Compila el proyecto asegurando que `sqlite3.c` esté incluido.
2. Ejecuta el programa.
3. El menú principal permite navegar entre todas las funciones:

Ingresar producto

Mostrar productos

Actualizar producto

Eliminar producto

Registrar ventas mensuales

Mostrar ventas registradas

Consultar productos con bajo stock

Consultar total de ventas por producto

Mostrar reporte anual de ventas

Salir

yaml
Copiar código

---

## 📊 Ejemplo del reporte anual

sql
Copiar código
                   IMPORTADORA RCE
           --REPORTE ANUAL DE VENTAS--
                 Tecpán, Guatemala
               Tel: 51828693
         higuerosroni77@gmail.com
Producto Ene Feb Mar Abr ... Dic | Total
Aceite motor 12 5 9 7 ... 11 | 73
Llantas 13" 3 1 4 0 ... 2 | 19
Total Mes 15 6 13 7 ... 13 | 92
yaml
Copiar código

---

## 🧩 Principales funciones internas

- `crearBaseDeDatos()` → Inicializa SQLite y crea tablas  
- `ingresarProducto()` → Insert CRUD  
- `mostrarProductos()` → Select con formato de tabla  
- `actualizarProducto()` → Update con verificaciones  
- `eliminarProducto()` → Delete con parámetros  
- `registrarVentasSQLite()` → Inserta ventas mensuales  
- `consultarProductosConBajoStock()` → Filtros por cantidad  
- `consultarTotalVentasPorProducto()` → Agrupación por producto  
- `mostrarReporteAnualDeVentas()` → Reporte con arreglos de 12 meses  

---

## 📌 Autor

Desarrollado por **Rony Samir Vicente Higueros**  
Proyecto académico — Universidad Mariano Gálvez  
Importadora RCE · Tecpán, Guatemala  

---


