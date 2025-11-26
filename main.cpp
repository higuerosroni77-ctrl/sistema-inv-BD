#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <map>
#include <array>   // Para std::array
#include "sqlite3.h"

using namespace std;

// Códigos ANSI para colores
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"
#define BOLD    "\033[1m"

sqlite3* db;

struct Producto {
    int codigoProducto;
    string nombreProducto;
    int cantidadExistente;
    double precioUnitario;
};

void crearBaseDeDatos() {
    int rc = sqlite3_open("inventario.db", &db);
    if (rc) {
        cerr << "No se pudo abrir la base de datos: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }

    const char* sqlCrearTabla =
        "CREATE TABLE IF NOT EXISTS productos ("
        "codigo INTEGER PRIMARY KEY,"
        "nombre TEXT NOT NULL,"
        "cantidad INTEGER NOT NULL,"
        "precio REAL NOT NULL);";

    char* errMsg;
    rc = sqlite3_exec(db, sqlCrearTabla, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al crear la tabla: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    const char* sqlCrearVentas =
        "CREATE TABLE IF NOT EXISTS ventas ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "producto_codigo INTEGER,"
        "mes INTEGER,"
        "cantidad INTEGER,"
        "FOREIGN KEY(producto_codigo) REFERENCES productos(codigo));";

    rc = sqlite3_exec(db, sqlCrearVentas, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error al crear la tabla de ventas: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void actualizarProducto() {
    int codigoBuscar;
    system("cls");
    cout << CYAN << "Ingrese el código del producto a actualizar: " << RESET;
    cin >> codigoBuscar;
    cin.ignore();

    // Consultar si el producto existe
    string sqlConsulta = "SELECT * FROM productos WHERE codigo = ?;";
    sqlite3_stmt* stmtConsulta;

    if (sqlite3_prepare_v2(db, sqlConsulta.c_str(), -1, &stmtConsulta, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmtConsulta, 1, codigoBuscar);

        if (sqlite3_step(stmtConsulta) == SQLITE_ROW) {
            Producto prod;
            prod.codigoProducto = codigoBuscar;
            cout << "Nuevo nombre del producto: ";
            getline(cin, prod.nombreProducto);
            cout << "Nueva cantidad: ";
            cin >> prod.cantidadExistente;
            cout << "Nuevo precio unitario: ";
            cin >> prod.precioUnitario;

            sqlite3_finalize(stmtConsulta);

            string sqlUpdate = "UPDATE productos SET nombre = ?, cantidad = ?, precio = ? WHERE codigo = ?;";
            sqlite3_stmt* stmtUpdate;

            if (sqlite3_prepare_v2(db, sqlUpdate.c_str(), -1, &stmtUpdate, nullptr) == SQLITE_OK) {
                sqlite3_bind_text(stmtUpdate, 1, prod.nombreProducto.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmtUpdate, 2, prod.cantidadExistente);
                sqlite3_bind_double(stmtUpdate, 3, prod.precioUnitario);
                sqlite3_bind_int(stmtUpdate, 4, prod.codigoProducto);

                if (sqlite3_step(stmtUpdate) == SQLITE_DONE) {
                    cout << GREEN << "Producto actualizado correctamente." << RESET << endl;
                } else {
                    cout << RED << "Error al actualizar el producto." << RESET << endl;
                }

                sqlite3_finalize(stmtUpdate);
            }
        } else {
            cout << RED << "Producto no encontrado." << RESET << endl;
            sqlite3_finalize(stmtConsulta);
        }
    }
}

void eliminarProducto() {
    int codigoEliminar;
    system("cls");
    cout << CYAN << "Ingrese el código del producto a eliminar: " << RESET;
    cin >> codigoEliminar;

    string sql = "DELETE FROM productos WHERE codigo = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, codigoEliminar);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << GREEN << "Producto eliminado exitosamente." << RESET << endl;
        } else {
            cout << RED << "Error al eliminar el producto." << RESET << endl;
        }

        sqlite3_finalize(stmt);
    } else {
        cout << RED << "No se pudo preparar la consulta de eliminación." << RESET << endl;
    }
}

void mostrarMenu() {
    system("cls");
    cout << BLUE << BOLD << "\n=== MENU PRINCIPAL ===\n" << RESET;
    cout << GREEN << "1. " << RESET << "Ingresar producto\n";
    cout << GREEN << "2. " << RESET << "Mostrar productos\n";
    cout << GREEN << "3. " << RESET << "Actualizar producto\n";
    cout << GREEN << "4. " << RESET << "Eliminar producto\n";
    cout << GREEN << "5. " << RESET << "Registrar ventas mensuales\n";
    cout << GREEN << "6. " << RESET << "Mostrar ventas registradas\n";
    cout << GREEN << "7. " << RESET << "Consultar productos con bajo stock\n";
    cout << GREEN << "8. " << RESET << "Consultar total de ventas por producto\n";
    cout << GREEN << "9. " << RESET << "Mostrar reporte anual de ventas\n";
    cout << GREEN << "10. " << RESET << "Salir\n";
    cout << YELLOW << "Seleccione una opcion: " << RESET;
}

void ingresarProducto() {
    Producto prod;
    system("cls");
    cout << CYAN << "\nIngrese los datos del producto:" << RESET << endl;
    cout << "Codigo del producto: ";
    cin >> prod.codigoProducto;
    cin.ignore();
    cout << "Nombre del producto: ";
    getline(cin, prod.nombreProducto);
    cout << "Cantidad en existencia: ";
    cin >> prod.cantidadExistente;
    cout << "Precio unitario: ";
    cin >> prod.precioUnitario;

    string sql = "INSERT INTO productos (codigo, nombre, cantidad, precio) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, prod.codigoProducto);
        sqlite3_bind_text(stmt, 2, prod.nombreProducto.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, prod.cantidadExistente);
        sqlite3_bind_double(stmt, 4, prod.precioUnitario);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cout << RED << "Error al guardar en la base de datos." << RESET << endl;
        } else {
            cout << GREEN << "Producto ingresado exitosamente.\n" << RESET;
        }
        sqlite3_finalize(stmt);
    } else {
        cout << RED << "No se pudo preparar la consulta." << RESET << endl;
    }
}

void mostrarProductos() {
    system("cls");
    const char* sql = "SELECT * FROM productos;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        cout << BLUE << BOLD << "\n=== LISTA DE PRODUCTOS ===\n" << RESET;
        cout << "------------------------------------------------------------" << endl;
        cout << "| Codigo | Nombre del Producto | Cantidad | Precio Unitario |" << endl;
        cout << "------------------------------------------------------------" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int codigo = sqlite3_column_int(stmt, 0);
            const unsigned char* nombre = sqlite3_column_text(stmt, 1);
            int cantidad = sqlite3_column_int(stmt, 2);
            double precio = sqlite3_column_double(stmt, 3);

            cout << "| " << setw(6) << codigo << " | "
                 << setw(20) << nombre << " | "
                 << setw(8) << cantidad << " | "
                 << setw(14) << precio << " |" << endl;
        }

        cout << "------------------------------------------------------------" << endl;
        sqlite3_finalize(stmt);
    } else {
        cout << RED << "Error al obtener los productos." << RESET << endl;
    }
}

void registrarVentasSQLite() {
    int codigo, mes, cantidad;
    system("cls");
    cout << CYAN << "Ingrese el código del producto vendido: " << RESET;
    cin >> codigo;
    cout << "Ingrese el mes de la venta (1-12): ";
    cin >> mes;
    cout << "Ingrese la cantidad vendida: ";
    cin >> cantidad;

    // Verificar que el producto existe
    string sqlCheck = "SELECT 1 FROM productos WHERE codigo = ?;";
    sqlite3_stmt* stmtCheck;
    bool existe = false;

    if (sqlite3_prepare_v2(db, sqlCheck.c_str(), -1, &stmtCheck, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmtCheck, 1, codigo);
        if (sqlite3_step(stmtCheck) == SQLITE_ROW) {
            existe = true;
        }
        sqlite3_finalize(stmtCheck);
    }

    if (!existe) {
        cout << RED << "El producto con ese código no existe. No se puede registrar la venta." << RESET << endl;
        return;
    }

    string sql = "INSERT INTO ventas (producto_codigo, mes, cantidad) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, codigo);
        sqlite3_bind_int(stmt, 2, mes);
        sqlite3_bind_int(stmt, 3, cantidad);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << GREEN << "Venta registrada correctamente." << RESET << endl;
        } else {
            cout << RED << "Error al registrar la venta." << RESET << endl;
        }
        sqlite3_finalize(stmt);
    } else {
        cout << RED << "Error al preparar la consulta SQL de venta." << RESET << endl;
    }
}

void mostrarVentasSQLite() {
    system("cls");
    const char* sql =
        "SELECT v.id, p.nombre, v.mes, v.cantidad "
        "FROM ventas v JOIN productos p ON v.producto_codigo = p.codigo;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        cout << BLUE << BOLD << "\n=== VENTAS REGISTRADAS ===\n" << RESET;
        cout << "-----------------------------------------------------" << endl;
        cout << "| ID | Producto           | Mes      | Cantidad     |" << endl;
        cout << "-----------------------------------------------------" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* nombre = sqlite3_column_text(stmt, 1);
            int mes = sqlite3_column_int(stmt, 2);
            int cantidad = sqlite3_column_int(stmt, 3);

            cout << "| " << setw(2) << id << " | "
                 << setw(18) << nombre << " | "
                 << setw(8) << mes << " | "
                 << setw(12) << cantidad << " |" << endl;
        }

        cout << "-----------------------------------------------------" << endl;
        sqlite3_finalize(stmt);
    } else {
        cout << RED << "Error al consultar las ventas." << RESET << endl;
    }
}

void consultarProductosConBajoStock(int limite) {
    system("cls");
    const char* sql = "SELECT * FROM productos WHERE cantidad <= ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, limite);

        cout << BLUE << BOLD << "\n=== PRODUCTOS CON BAJO STOCK ===\n" << RESET;
        cout << "------------------------------------------------------------" << endl;
        cout << "| Codigo | Nombre del Producto | Cantidad | Precio Unitario |" << endl;
        cout << "------------------------------------------------------------" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int codigo = sqlite3_column_int(stmt, 0);
            const unsigned char* nombre = sqlite3_column_text(stmt, 1);
            int cantidad = sqlite3_column_int(stmt, 2);
            double precio = sqlite3_column_double(stmt, 3);

            cout << "| " << setw(6) << codigo << " | "
                 << setw(20) << nombre << " | "
                 << setw(8) << cantidad << " | "
                 << setw(14) << precio << " |" << endl;
        }

        cout << "------------------------------------------------------------" << endl;
        sqlite3_finalize(stmt);
    } else {
        cout << RED << "Error al consultar productos con bajo stock." << RESET << endl;
    }
}

void consultarTotalVentasPorProducto() {
    system("cls");
    const char* sql =
        "SELECT p.codigo, p.nombre, SUM(v.cantidad) as total_vendido "
        "FROM ventas v JOIN productos p ON v.producto_codigo = p.codigo "
        "GROUP BY p.codigo, p.nombre;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        cout << BLUE << BOLD << "\n=== TOTAL DE VENTAS POR PRODUCTO ===\n" << RESET;
        cout << "-------------------------------------------------" << endl;
        cout << "| Codigo | Nombre del Producto | Total Vendido |" << endl;
        cout << "-------------------------------------------------" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int codigo = sqlite3_column_int(stmt, 0);
            const unsigned char* nombre = sqlite3_column_text(stmt, 1);
            int total = sqlite3_column_int(stmt, 2);

            cout << "| " << setw(6) << codigo << " | "
                 << setw(20) << nombre << " | "
                 << setw(13) << total << " |" << endl;
        }

        cout << "-------------------------------------------------" << endl;
        sqlite3_finalize(stmt);
    } else {
        cout << RED << "Error al consultar total de ventas." << RESET << endl;
    }
}

void mostrarReporteAnualDeVentas() {
    system("cls");

    const char* sql =
        "SELECT p.nombre, v.mes, SUM(v.cantidad) "
        "FROM ventas v "
        "JOIN productos p ON v.producto_codigo = p.codigo "
        "GROUP BY p.nombre, v.mes "
        "ORDER BY p.nombre, v.mes;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cout << RED << "Error al preparar consulta para reporte anual." << RESET << endl;
        return;
    }

    // Mapeo de productos y sus ventas mensuales (12 meses)
    map<string, array<int, 12>> ventasPorProducto;
    int totalMensual[12] = {0};
    int totalGeneral = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        string nombre = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int mes = sqlite3_column_int(stmt, 1);
        int cantidad = sqlite3_column_int(stmt, 2);

        if (mes >= 1 && mes <= 12) {
            auto &ventas = ventasPorProducto[nombre]; // array<int,12> inicializado a 0
            ventas[mes - 1] += cantidad;
            totalMensual[mes - 1] += cantidad;
            totalGeneral += cantidad;
        }
    }

    sqlite3_finalize(stmt);

    // Imprimir el reporte
    string meses[12] = {"Ene", "Feb", "Mar", "Abr", "May", "Jun",
                        "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};

    cout << BLUE;
    cout << setw(50) << right << "IMPORTADORA RCE" << endl;
    cout << setw(50) << right << "--REPORTE ANUAL DE VENTAS--" << endl;
    cout << setw(50) << right << "Tecpán, Guatemala" << endl;
    cout << setw(50) << right << "Tel: 51828693" << endl;
    cout << setw(50) << right << "higuerosroni77@gmail.com" << endl;
    cout << RESET;

    cout << BLUE << "======================================================================" << endl;
    cout << setw(40) << "INVENTARIO DE VENTAS ANUAL, MENSUAL" << endl;
    cout << "======================================================================" << RESET << endl;

    cout << left << setw(15) << "Producto" << "|";
    for (int i = 0; i < 12; i++) {
        cout << setw(4) << meses[i];
    }
    cout << "| Total A" << endl;
    cout << "---------------------------------------------------------------------" << endl;

    for (const auto& par : ventasPorProducto) {
        const string& nombre = par.first;
        const auto& ventas = par.second;
        int totalProducto = 0;

        cout << left << setw(15) << nombre << "|";
        for (int i = 0; i < 12; i++) {
            cout << setw(4) << ventas[i];
            totalProducto += ventas[i];
        }
        cout << "| " << totalProducto << endl;
    }

    cout << "---------------------------------------------------------------------" << endl;
    cout << left << setw(15) << "Total Mes" << "|";
    for (int i = 0; i < 12; i++) {
        cout << setw(4) << totalMensual[i];
    }
    cout << "| " << totalGeneral << endl;
    cout << BLUE << "======================================================================" << RESET << endl;
}

int main() {
    crearBaseDeDatos();
    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
        case 1:
            ingresarProducto();
            break;
        case 2:
            mostrarProductos();
            break;
        case 3:
            actualizarProducto();
            break;
        case 4:
            eliminarProducto();
            break;
        case 5:
            registrarVentasSQLite();
            break;
        case 6:
            mostrarVentasSQLite();
            break;
        case 7: {
            int limite;
            cout << CYAN << "Ingrese el límite de stock: " << RESET;
            cin >> limite;
            consultarProductosConBajoStock(limite);
            break;
        }
        case 8:
            consultarTotalVentasPorProducto();
            break;
        case 9:
            mostrarReporteAnualDeVentas();
            break;
        case 10:
            cout << YELLOW << "Saliendo del programa.\n" << RESET;
            break;
        default:
            cout << RED << "Opción no válida. Intente de nuevo.\n" << RESET;
        }

        if (opcion != 10) {
            system("pause");
        }
    } while (opcion != 10);

    sqlite3_close(db);
    return 0;
}

