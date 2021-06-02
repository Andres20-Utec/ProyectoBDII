# Base de Datos 2: Proyecto 1

Integrantes:

* Nicho, Jorge
* Riveros, Andres
* Tanta, Johan

## Introducción
La tecnología una parte fundamental en la solución de los problemas diversos que vivimos 
en la actualidad. En consecuencia, uno de los problemas que siempre ha existido en las universidades es el mantenimiento de su intranet, en especial el sistema de matrícula. 
Prueba de ello, es que a mediados del anterior año 2020 e intensificado por una mayor cantidad de estudiantes conectados y siendo admitidos a universidades, estas han dado mayor mantenimiento a sus plataformas por fallas en los servidores debido a la alta concurrencia de los estudiantes que no fueron previamente contempladas.

Esto ha promovido que universidades como UTEC mejoren su sistema y requieran cuantificar a los estudiantes para exigir la robustez de los servidores.

El objetivo de este proyecto es realizar un gestor de base de datos con interfaz gráfica, utilizando operaciones de búsqueda, búsqueda por rango, inserción y eliminación, usando dos técnicas de organización de archivos: Sequential File y Extendible Hashing.

El dominio de datos esta basado en los estudiantes de la Universidad de Ingeniería y Tecnología - UTEC. Los campos son: Código, Nombre y Carrera.

Los resultados fueron satisfactorios ya que pudimos crear un gestor de base de datos con una interfaz amigable para el usuario para ambas técnicas de organización de archivos. Este también fue puesto a prueba para medir su desempeño en las diferentes operaciones, y posteriormente fue analizado.


## Consideraciones
En primer lugar, el **Sequential File** escribe en el datafile la position y referencia
del menor registro a comparación de los demás, y una variable booleana que representa si se han
eliminado registros anteriormente. Asimismo, se implementó como template.
```
template <class Register, class Key>
class SequentialFile;
```
Para poder utilizar nuestro **Sequential File**, se debe definir el tipo de registro y el tipo de dato
de su llave primaria. Es decir, si estamos trabajando con la entidad _Estudiante_ y su llave primaria
es un arreglo de caracteres. Asimismo, en su constructor, asignamos la ruta del archivo de datos y
del auxiliar, y la capacity del archivo auxiliar.
```
SequentialFile<Estudiante, const char*> mySequentialFile(dataFilePath, auxFilePath, capacity);
```
También, el tipo de registro debe de tener los siguientes atributos en public
```
int nextDel
char reference
```
Las siguientes funciones implementadas por dentro
```
Key getPrimaryKey();
bool equalToKey(Key key);
bool lessThanToKey(Key key);
bool greaterThanToKey(Key key);
bool lessThanOrEqualToKey(Key key);
bool greaterThanOrEqualToKey(Key key);
```
Y las siguientes sobrecargas de operadores
```
bool operator < (Register& other);
bool operator == (Register& other);
```

Por otro lado, la idea de construcción del **Extendible Hashing** es la de un árbol
binario con una altura máxima y; asimismo, se diseñó como template.
```
template <class Register, class Key, class Hash = hash<Key>> 
class HashIndex;
```
Es decir, se tiene que definir el tipo de registro, el tipo de dato de la llave
primaria y, de manera opcional, un hash. Por lo que, si trabajamos con la entidad
_Alumno_, el cual su llave primaria es un arreglo de caracteres y definimos su propio hash,
este último es una clase o estructura el cual sobrecargan el operador (). Asimismo, en el
constructor asignamos la ruta del archivo de datos y del auxiliar, y la altura máxima del árbol.
```
HashIndex<Alumno, const char *, AlumnoHash> myHashIndex(dataFilePath, dataFileAux, maxHeight);
```
Además, al trabajar con los archivos donde se encuentran los buckets y los nodos
que son propios del índice, se necesita trabajar con dos _FreeList_:
```
template <class Register, class Key, class Hash = hash<Key> >
class HashIndex{
private:
    typedef Bucket<Register, Key> Bucket;
    FreeList<Bucket> bucketFile;
    FreeList<HashNode> indexFile;
    Hash myHash;
}
```
De igual forma, el tipo de registro debe contener los atributos, funciones y
sobrecarga de operadores mencionados anteriormente.
## Descripción de los métodos
### Sequential File
<div style="text-align: center"> Función de inserción</div>

```
void add(Register record);
```
#### Procedimiento
* Si el archivo de datos se encuentra vacío, se procede a escribir la información del primer
  registro, el status de registros eliminados en falso y el registro como tal en el
  archivo de datos. 

* Luego de verificar ello, se corrobora que el archivo auxiliar no se encuentre lleno;
  es decir, el tamaño es igual a su capacidad máxima. Si es así, se reconstruye el
  archivo.

* Después, leemos el status de archivos eliminados que se encuentra guardado en el archivo
  de datos. Si el status es false, se utiliza la búsqueda binaria. En caso contrario se
  utiliza la búsqueda secuencial para encontrar el registro que apuntaría a este nuevo.

* Finalmente, se realizan los cambios de los atributos _nextDel_ y _reference_
  respectivamente tanto del registro nuevo a insertar y del registro que apunta al nuevo.
  * El primer caso a considerar es cuando se inserta un registro con una llave que
    es menor al primer registro por lo que hay que actualizar la información de la posición
    y referencia del primer registro que sería este nuevo en el archivo de datos y se escribe
    este registro en el archivo auxiliar.
  * El segundo caso, es cuando todos los registros se han eliminado. En este escenario,
    el nuevo registro a insertar será el primer registro, por lo que se guarda su información
    respectiva (posición y referencia) en el archivo de datos y el registro se escribe en el
    archivo auxiliar.
* Cada actualización de los atributos de un registro se debe escribir nuevamente en
  su archivo correspondiente.

<div style="text-align: center"> Función de búsqueda </div>

```
vector<Register> search(Key key);
```
#### Procedimiento
* Se verifica si es posible utilizar la búsqueda binaria, esto se da cuando no
existan registros eliminados y el archivo auxiliar se encuentre vacío. En caso contrario
  se utiliza la búsqueda secuencial.

* En la búsqueda secuencial, si hay registros en el archivo auxiliar, entonces es necesario
  realizar el salto entre archivos. Esto no es necesario en la búsqueda binaria, ya que
  solo se aplica el archivo de datos. Estas funciones utilizan la posición lógica de cada
  registro, pero al leerlas se utiliza su posición física.

* Esta función devuelve todas los registros con la misma key de búsqueda.

<div style="text-align: center"> Función de búsqueda por rango</div>

```
vector<Register> searchPerRange(Key startKey, Key endKey);
```
#### Procedimiento
* De manera análoga, se verifica si es posible utilizar la búsqueda binaria. En
  caso contrario se utiliza la búsqueda secuencial.

* Al utilizar la búsqueda binaria, ubicamos el límite inferior y el superior. Luego,
  guardamos todos los registros de ese intervalo y este sería la respuesta.

* Al utilizar la búsqueda secuencial, solo ubicamos el límite inferior. Luego, se
  recorre tanto realizando saltos, respectivamente, entre el archivo de datos y el auxiliar
  verificando si el registro pertenece al intervalo.

* En las funciones de búsqueda utiliza la posición lógica de cada registro, pero al
  leer el registro se utiliza su posición física.

<div style="text-align: center"> Función de eliminación </div>

```
void removeRecord(Key keyToRemove)
```
#### Procedimiento
* En primer lugar, si nuestro archivo de datos y el auxiliar se encuentran vacíos,
entonces, no existe algo que remover.

* Luego, verificamos que si la key existe. En caso contrario, no se realiza ninguna
  acción.

* Después, ubicamos el registro que apunta al registro a eliminar para que apunte
  al registro siguiente.
  * En caso que el registro a eliminar sea el primer registro, se escribe en el archivo
    de datos la información correspondiente del primer registro, que sería lo que apunta el registro
    a eliminar.

* Finalmente, actualizamos el atributo _nextDel_ a _-1_ y _reference_ a _'x'_ del
  registro a eliminar y lo escribimos en el archivo correspondiente y; asimismo, escribimos
  en el archivo de datos el status de archivos eliminados en true.

* Cada actualización de los atributos de los registros, se debe escribir nuevamente en su
  archivo correspondiente.
### Extendible Hashing
<div style="text-align: center"> Función de inserción </div>

```
void insert(Register record);
```
#### Procedimiento
* En primer lugar, si el archivo de buckets se encuentra vacío se procede a crear y escribir, 
  en sus archivos correspondientes, 3 nodos (el root, su hijo izquierdo y su derecho) 
  y dos buckets.

* Luego de aplicar el hash a la llave primaria del registro, este resultado, es expresado
  en binario, el cual '0' representa hijo izquierdo y '1' hijo derecho, por lo que nos movemos
  en el árbol hasta identificar el nodo hoja.

* Después de identificarlo, se inserta este registro en el bucket que apunta este nodo.
  * En caso que el bucket este lleno y la altura del nodo hoja no es la máxima establecida,
    se procede a realizar un split y colocar en los lugares correspondientes los registros
    que se encuentran en el bucket y el registro a insertar.
  * En caso de que el nodo hoja se encuentre en la altura máxima, se procede a crear un nuevo
    bucket el cual guarda este nuevo registro y se empieza a enlazar los buckets
    correspondientes.

* Cada modificación de algún atributo de un bucket o nodo, se debe escribir en la posicion y archivo
  correspondiente.

<div style="text-align: center"> Función de búsqueda </div>

```
vector<Register> search(Key searchKey);
```
#### Procedimiento
* Se aplica la función hash a la llave de búsqueda y con su expresión binaria localizamos el nodo
hoja. Asimismo, cada nodo no hoja, tiene la posición lógica de su hijo izquierdo y derecho en el archivo
  del índice, por lo que es útil para obtener su posición física.

* Luego, leemos el bucket que apunta este nodo hoja. Inmediatamente, revisamos cada registro del
  bucket y guardamos en un vector los que tenga la misma llave. Este proceso se repite, si existe
  enlazamiento de buckets.

<div style="text-align: center"> Función de eliminación </div>

```
void remove(Key key);
```
* Se aplica la función hash a la llave que es pasada como argumento y con su expresión binaria
localizamos el nodo hoja.

* Leemos el bucket y guardamos en un vector auxiliar todos los registros que sean diferente de
  la llave. Inmediatamente, los registros del vector son guardados en el bucket.

* En caso que un nodo 'v' no hoja, tanto el bucket que apunta su hijo izquierdo y su derecho son vacíos,
  se procede a eliminar sus hijos y asignar un bucket al nodo 'v', el cual se convierte en un nodo hoja. Este
  proceso se repite de manera recursiva, pero no se aplica para el nodo raíz.

## Experimentos
Se define nuestro dominio de datos, en este caso, se utilizó la entidad Universitario.h, el cual tiene los siguientes atributos:

```
class Universitario{
private:
    char codigo[5];
    char nombre[11];
    char carrera[15];
}
```
Para la realización de experimentos, se creó datasets de tamaño 1k, 5k, 10k, 50k y 100k.
Los cuales están guardados en un archivo .csv por lo que es necesario diseñar un parser
que lea estos archivos.

## Comparación entre técnicas de indexación
### Tiempo de ejecución
Se ejecutó 5 veces la función de inserción del Sequential File y Extendible Hashing y se obtuvo 
el promedio de este. Asimismo, esto fue ejecutado en los diferentes datasets creados.

#### * Ejecución de la función de inserción
En la siguiente gráfica se observa el tiempo promedio de la función
de inserción del Sequential File.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_T2.png)


En esta gráfica, se realiza el mismo proceso utilizando el Extendible Hashing.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_T4.png)

En resumen, estos fueron los resultados:

| Dataset | Tiempo promedio (µs) Sequential File | Tiempo promedio (µs) Extendible Hashing |
|:-------:|:------------------------------------:|:---------------------------------------:|
|    1k   |                  56                  |                   87,8                  |
|    5k   |                 55,8                 |                  106,6                  |
|   10k   |                  59                  |                   104                   |
|   50k   |                 78,2                 |                  126,4                  |
|   100k  |                  68                  |                  137,2                  |

#### * Ejecución de la función de búsqueda
En la siguiente gráfica se observa el tiempo promedio de la función
de búsqueda del Sequential File.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_T1.png)

En esta gráfica, se realiza el mismo proceso utilizando el Extendible Hashing.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_T3.png)

En resumen, estos fueron los resultados:

| Dataset | Tiempo promedio (µs) Sequential File | Tiempo promedio (µs) Extendible Hashing |
|:-------:|:------------------------------------:|:---------------------------------------:|
|    1k   |                1383,4                |                  103,4                  |
|    5k   |                7754,8                |                   138                   |
|   10k   |                14625,8               |                  130,6                  |
|   50k   |                72954,6               |                  177,6                  |
|   100k  |               137516,6               |                  160,2                  |

### Cantidad de accesos a memoria secundaria

Se ejecutó 5 veces la función de inserción del Sequential File y Extendible Hashing y se obtuvo
la cantidad de accesos a disco. Asimismo, esto fue ejecutado en los diferentes datasets creados.

#### * Ejecución de la función de inserción
En la siguiente gráfica se observa la cantidad de accesos a memoria secundaria de la 
función de inserción del Sequential File y extendible Hashing.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_AMS1.png)

En resumen, estos fueron los resultados:

| Dataset | Cantidad de accesos a disco Sequential File | Cantidad de accesos a disco Extendible Hashing |
|:-------:|:-------------------------------------------:|:----------------------------------------------:|
|    1k   |                      16                     |                      11,4                      |
|    5k   |                     18,2                    |                      13,8                      |
|   10k   |                     19,8                    |                      14,4                      |
|   50k   |                      22                     |                      16,8                      |
|   100k  |                      23                     |                      18,4                      |

#### * Ejecución de la función de búsqueda
En la siguiente gráfica se observa la cantidad de accesos a memoria secundaria de la
función de búsqueda del Sequential File.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_AMS2.png)

En esta gráfica, se realiza el mismo proceso utilizando el Extendible Hashing.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/Grafica_AMS3.png)

En resumen, estos fueron los resultados:

| Dataset | Cantidad de accesos a disco Sequential File | Cantidad de accesos a disco Extendible Hashing |
|:-------:|:-------------------------------------------:|:----------------------------------------------:|
|    1k   |                     1009                    |                       14                       |
|    5k   |                     5009                    |                       18                       |
|   10k   |                    10009                    |                       19                       |
|   50k   |                    50009                    |                       23                       |
|   100k  |                    100009                   |                       22                       |

## Análisis
* Al llenarse el archivo auxiliar del Sequential File, este debe de reconstruir el archivo
nuevamente. Debido a ello, resulta que a veces la inserción demora más de lo normal. Por
lo que es necesario definir la capacidad del archivo auxiliar en un punto intermedio. Es decir, 
  no debería ser una capacidad muy pequeña, ya que al haber muchas inserciones, se estaría
reconstruyendo varias veces y degradando la performance. Desde el punto contrario, con
una capacidad muy alta, se evita la reconstrucción a cada momento, pero al realizar varias
búsquedas, no se aprovecha la búsqueda binaria debido a que existe registros en el archivo
auxiliar por lo que se opta la búsqueda secuencial que es deficiente.

* Se observa en los resultados que la inserción en el sequential file resulta mejor que en el
Hash y esto se debe a un escenario. Esto es, que al insertar un registro con una llave
primaria que resulta ser insertada en las primeras posiciones, por lo que no toma mucho
tiempo. En el caso del Extendible Hashing, va a depender del resultado de aplicar la función
hash a la llave, ya que con este resultado identificamos el nodo hoja correspondiente.
Asimismo, en este proceso, que además toma tiempo, tiene el factor de que si el bucket se
encuentra completo se tiene que realizar un split, es decir crear y escribir dos nodos y dos
buckets.

* El Extendible Hashing depende de una máxima altura, la cual debe ser definida en un punto
intermedio. Es decir, si se define en una altura muy pequeña, al haber gran cantidad de
inserciones, el árbol se completa por lo que se procede a enlazar páginas lo cual degrada la
búsqueda. Esto es debido a que la búsqueda secuencial prevalece ante la búsqueda
binaria. Desde el punto contrario, con una altura muy grande y al haber gran cantidad de
inserciones, sucede que al completar el bucket que apunta un nodo hoja, se debe realizar
un split. En este proceso, se actualiza el nodo padre y se crea dos nodos hijos y sus
buckets por lo que es necesario acceder al disco en varias ocasiones. Esto significa que
degrada el proceso de inserción, pero aumenta el rendimiento de la búsqueda, ya que
prevalece la búsqueda binaria.


## QT

Realizamos una interface para presentar la tabla de datos usando ambas técnicas y con una actualización de la tabla en tiempo real para cada una de las operaciones.

Declarando variables y funciones para el MainWIndow

### Sequential File

Tenemos el MainWindow, que es la ventana principal del SGBD, en donde se van a mostrar todas las operaciones del Sequential File.

```
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(3);
    csvModel->setHorizontalHeaderLabels(QStringList()<< "Codigo" << "Nombre" << "Carrera");
    ui->tableView->setModel(csvModel);
    ui->outputCodigo->setText("");
    ui->outputNombre->setText("");
    ui->outputCarrera->setText("");
}
```

Botón carga inicial: Carga los registros iniciales.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT2_INITIAL_DATA.png)

Botón actualizar: Imprime la tabla csv en la interface, y nos permite mostrarla luego de cada operación.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT2_ACTUALIZAR.png)

Botón Insertar: Inserta el registro en el Sequential File a partir de operaciones localizadas en un SequentialController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT2_INSERT.png)

Botón search: Busca el registro en el Sequential File a partir de operaciones localizadas en un SequentialController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT2_SEARCH.png)

Botón searchRange: Busca los registros entre una key de inicio y una de final en el Sequential File a partir de operaciones localizadas en un SequentialController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT2_SEARCH_RANGE.png)

Botón delete: Busca el registro indicado y lo elimina en el Sequential File a partir de operaciones localizadas en un SequentialController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT2_DELETE.png)

Por último creamos un SequentialController para manipular las operaciones del Sequential File desde cualquier lugar.

```
class SequentialController{
public:
    
    ...

    void insertAllTest(){
        printTestStart(R"(insertAll("Andres", "Sagasti", "Claudia"))");
        vector<Universitario> records{
            Universitario("P-11", "Andres", "cs"),
            Universitario("P-12", "Sagasti", "cs"),
            Universitario("P-13", "Claudia", "cs"),
            Universitario("P-42","Daniela","cs"),
            Universitario("P-51","Rosario", "bio"),
            Universitario("P-23","Johana","bio"),
            Universitario("P-78", "Osman", "bio"),
            Universitario("P-32","Jorge", "bio"),
            Universitario("P-26", "Gerson", "cs"),
            Universitario("P-39", "Gabriela", "cs")};

        test.insertAll(records);
        //test.printAll();
        printTestEnd();
    }

    void addTest(string codigo, string nombre, string carrera){
        printTestStart("add()");
        insertByDefault(codigo, nombre, carrera);
        //test.printAll();
        printTestEnd();
    }

    vector<Universitario> searchTest(string nombre){
        printTestStart(R"(search())");
        cout << "Hey" << endl;
        auto universitarios = test.search(nombre.c_str());
        cout << "Baby" << endl;
        cout << universitarios[0].getNombre() << endl;
        //showAll(records);
        printTestEnd();
        return universitarios;
    }

    vector<Universitario> searchPerRange(string begin, string end){
        printTestStart(R"(searchPerRange())");
        auto records = test.searchPerRange(begin.c_str(), end.c_str());
        //showAll(records);
        printTestEnd();
        return records;
    }

    void deleteTest(string name){
        printTestStart(R"(delete()");
        test.removeRecord(name.c_str());
    }

    ...

};
```

### Extendible Hashing (Método 2)

Tenemos el MainWindow, que es la ventana principal del SGBD, en donde se van a mostrar todas las operaciones del Extendible Hashing.

```
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(6);
    csvModel->setHorizontalHeaderLabels(QStringList()<< "Codigo" << "Nombre" << "Apellidos" << "Carrera" << "Ciclo" << "Mensualidad");
    ui->tableView->setModel(csvModel);
    ui->outputCodigo->setText("");
    ui->outputNombre->setText("");
    ui->outputApellidos->setText("");
    ui->outputCarrera->setText("");
    ui->outputCiclo->setText("");
    ui->outputMensualidad->setText("");
}
```

Botón actualizar: Imprime la tabla csv en la interface, y nos permite mostrarla luego de cada operación.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT_ACTUALIZAR.png)

Botón Insertar: Inserta el registro en el extendible hash a partir de operaciones localizadas en un hashController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT_INSERT.png)

Botón search: Busca el registro en el extendible hash a partir de operaciones localizadas en un hashController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT_SEARCH.png)

Botón searchRange: Busca los registros entre una key de inicio y una de final en el extendible hash a partir de operaciones localizadas en un hashController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT_SEARCH_RANGE.png)

Botón delete: Busca el registro indicado y lo elimina en el extendible hash a partir de operaciones localizadas en un hashController.

![enter image description here](https://github.com/Andres20-Utec/ProyectoBDII/blob/main/Imagenes/QT_DELETE.png)

Por último creamos un HashController para manipular las operaciones del extendible hash desde cualquier lugar.

```
class HashController{
public:

    ...
    
    void insertTest(string codigo, string nombre, string apellidos, string carrera, int ciclo, float mensualidad) {
        printTestStart("insert()");
        insertByDefault(codigo, nombre, apellidos, carrera, ciclo, mensualidad);
        printTestEnd();
    }

    vector<Alumno> searchTest(string codigo){
        printTestStart(R"(search())");
        vector<Alumno> output = test.search(codigo.c_str());
        printTestEnd();
        return output;
    }

    vector<Alumno> searchPerRangeTest(string begin,string end){
        printTestStart(R"(serachPerRange())");
        vector<Alumno> output = test.searchInRange(begin.c_str(), end.c_str());
        printTestEnd();
        return output;
    }


    void deleteTest(string codigo){
        printTestStart("Delete function");
        test.remove(codigo.c_str());
        printTestEnd();
    }

};
```

## Video

Se encuentra aqui (ya que github no tenia mas almacenamiento): https://drive.google.com/drive/folders/13KDenkNmvols-dFAmqYdjQmhtmrkp7Yw?usp=sharing.
