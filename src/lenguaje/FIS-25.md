# Arquitectura del Sistema
La arquitectura FIS-25 opera sobre un modelo de ejecución secuencial.
El sistema no distingue entre mayúsculas y minúsculas en su conjunto de instrucciones.
La memoria se gestiona mediante declaraciones explícitas de variables y punteros para estructuras dinámicas.
El subsistema de visualización consiste en una matriz monocromática de 64 por 64 píxeles.
Este periférico integrado permite la visualización directa de algoritmos gráficos y matemáticos, proporcionando retroalimentación inmediata sobre la ejecución lógica del programa.

# Sistema de Tipos
El lenguaje intermedio soporta un conjunto estricto de tipos de datos primitivos necesarios para el cómputo numérico y lógico.

## Tabla de Tipos de Datos Primitivos
| Tipo | Descripción Semántica |
| ---- | --------------------- |
| int | Números enteros con signo. Utilizados para contadores e índices |
| float | Números reales de punto flotante. Esenciales para cálculos de precisión |
| bool | Valor lógico binario donde 0 representa Falso y 1 representa Verdadero |
| string | Cadenas de caracteres literales. Su uso se limita a instrucciones de salida |
| pointer | Referencia de memoria utilizada para arrays y listas dinámicas |
| --- | --- |

# Conjunto de Instrucciones (ISA)
Las instrucciones siguen una estructura de tres direcciones para operaciones aritméticas y lógicas, facilitando la traducción desde árboles de sintaxis abstracta (AST).

## Gestión de Memoria y Asignación
La gestión de estado se realiza mediante la declaración y asignación de variables. Las variables
declaradas fuera del ámbito de una función se consideran globales, mientras que las internas
se gestionan en el marco de pila actual.

| --- | --- |
| VAR <nombre> | Reserva espacio en memoria para una variable. |
| ASSIGN <valor> <dest> | Copia un valor literal o el contenido de una variable origen al destino. |
| --- | --- |

## Operaciones Aritméticas
La Unidad Aritmética Lógica (ALU) procesa operaciones matemáticas estándar. Es responsabilidad del programador o del compilador asegurar que los tipos de datos sean compatibles.

| --- | --- | --- |
| Instrucción | Operandos | Efecto |
| ADD | op1 op2 dest | Suma op1 y op2, almacena en dest. |
| SUB | op1 op2 dest | Resta op2 de op1, almacena en dest. |
| MUL | op1 op2 dest | Multiplica op1 por op2. |
| DIV | op1 op2 dest | Divide op1 entre op2. |
| MOD | op1 op2 dest | Calcula el módulo de la división. |
| POW | op1 op2 dest | Eleva op1 a la potencia op2. |
| --- | --- | --- |

## Control de Flujo y Lógica
El control de flujo se maneja mediante saltos incondicionales y condicionales hacia etiquetas predefinidas. Las comparaciones lógicas almacenan su resultado en variables booleanas que posteriormente son evaluadas por las instrucciones de ramificación.

__Definición de Etiquetas__: La instrucción LABEL <nombre> marca una dirección de memoria de
programa específica.

__Ramificación__:
- GOTO <label> realiza un salto incondicional.
- IF <cond> GOTO <label> salta si la condición es verdadera.
- IFFALSE <cond> GOTO <label> salta si la condición es falsa.

Las comparaciones se realizan mediante las instrucciones EQ (igual), NEQ (no igual), GT (mayor que), GTE (mayor o igual), LT (menor que) y LTE (menor o igual). Todas ellas toman dos operandos y un destino para el resultado booleano.

## Subrutinas y Pila
El sistema implementa llamadas a funciones mediante una pila de parámetros explícita. Antes de invocar una subrutina con GOSUB, los argumentos deben ser apilados usando PARAM. Dentro de la subrutina, PARAM_GET recupera estos valores. La instrucción RETURN devuelve el control al punto de llamada.

# Entrada y Salida (I/O)
La interacción con el mundo exterior se limita a la consola de texto y la matriz gráfica.

## Tabla 3: Instrucciones de I/O
| --- | --- |
| Instrucción | Funcionalidad |
| PRINT <val> | Escribe el valor en la salida estándar. |
| INPUT <dest> | Detiene la ejecución hasta recibir entrada del usuario. |
| PIXEL <x> <y> <c> | Modifica el estado (0/1) del píxel en (x, y). |
| KEY <k> <dest> | Consulta el estado de una tecla específica (polling). |
| --- | --- |