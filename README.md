# Base de Datos 2: Proyecto 1

Integrantes:

* Nicho, Jorge
* Riveros, Andres
* Tanta, Johan

## Introducción
La tecnología una parte fundamental en la solución de los problemas sanitarios que vivimos 
en la actualidad. En consecuencia, uno de los ámbitos que han incrementado exponencialmente 
su impacto y su promoción en la sociedad, considerado como una "alternativa segura" de diversión, 
son los videojuegos. Tanto es así, que gran cantidad de jugadores casuales y targets imprevistos 
se han unido a esta forma de entretenimiento. Prueba de ello, es que a mediados del presente año 2020, 
2 juegos casuales "FallGuys" y "AmongUs" (desarrollado el 2016 para plataformas móviles) 
han dado mantenimiento a sus plataformas por fallas en los servidores por la alta concurrencia de jugadores no contemplada. 
Esto ha promovido que plataformas de distribución de juegos como STEAM presenten catálogos 
extendidos y requieran cuantificar a los jugadores por juego para exigir la robustez de los 
servidores tanto propios como de los juegos de su catálogo.

El objetivo de este proyecto es realizar un gestor de base de datos con interfaz gráfica, utilizando
operaciones de búsqueda, búsqueda por rango, inserción y eliminación, usando dos técnicas de organización 
de archivos: Sequential File y Extendible Hashing.

El dominio de datos esta basado en la plataforma digital de videojuegos, STEAM. 
Los campos son: Id, Nombre, Descripción y Horas Jugadas.

Los resultados fueron satisfactorios ya que pudimos crear un gestor de base de datos con una 
interfaz amigable para el usuario para ambas técnicas de organización de archivos. Este también fue 
puesto a prueba para medir su desempeño en las diferentes operaciones, y posteriormente fue analizado.

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
es un arreglo de caracteres, la declaración es la siguiente:
```
SequentialFile<Estudiante, const char*> mySequentialFile;
```
También, el tipo de registro debe de tener los siguientes atributos en públic
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
_Alummno_, el cual su llave primaria es un arreglo de caracteres y definimos su propio hash,
este último es una clase o estructura el cual sobrecargan el operador ().
```
HashIndex<Alumno, const char *, AlumnoHash> myHashIndex;
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
```
void add(Register record);
```
#### Procedimiento
* Si el datafile se encuentra vacío, se procede a escribir la información del primer
  registro, el status de registros eliminados en false y el registro como tal en el
  datafile. 
  
* Luego de verificar ello, se corrobora que el archivo auxiliar no se encuentre lleno;
  es decir, el tamaño es igual a su capacidad máxima. Si es así, se reconstruye el
  archivo.
  
* Después, leemos el status de archivos eliminados en el archivo de datos o en el 
  auxiliar. Si el status es false, se utiliza se manera directa la busqueda binaria y se
  realizan los cambios de los atributos _nextDel_ y _reference_ respectivamente tanto del
  registro nuevo a insertar y del registro que apunta al nuevo.  
  
* En caso contrario, al utilizar la busqueda secuencial, antes de insertar se debe tener
  en cuenta los casos cuando se han eliminado registros.

```
void search(Key key);
```
### Procedimiento