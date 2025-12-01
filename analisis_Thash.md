# Análisis de tablas de dispersión

* *Pablo Javier Montoro Bermúdez pjmb0003@red.ujaen.es*
* *Lázaro Ruiz Fernández lrf00011@red.ujaen.es*

## Tamaño de _tablaHash: 5099
| función               | máximo de colisiones | supera 10 colisiones |  factor de carga  | promedio de colisiones |
|-----------------------|:--------------------:|:--------------------:|:-----------------:|:----------------------:|
| _funcionHashNumeroUno |          38          |          89          |     0.649147      |        1.39607         |
| _funcionHashNumeroDos |          13          |          3           |     0.649167      |        0.765861        |
| _funcionHashNumeroTres|          12          |          1           |     0.649147      |        0.544713        |

## Tamaño de _tablaHash: 4871
| función                | máximo de colisiones | supera 10 colisiones | factor de carga | promedio de colisiones |
|------------------------|:--------------------:|:--------------------:|:---------------:|:----------------------:|
| _funcionHashNumeroUno  |          43          |          87          |    0.679532     |        1.36737         |
| _funcionHashNumeroDos  |          14          |          6           |    0.679532     |        0.830816        |
| _funcionHashNumeroTres |          13          |          3           |    0.679532     |        0.593656        |

## Justificación de la configuración elegida
Hemos optado por la función de dispersión doble número tres (funcionHashNumeroTres) ,
con un factor de carga λ = 0.65, ya que, según los resultados obtenidos, es la configuración que ofrece el mejor equilibrio entre 
eficiencia y estabilidad. 
En ambos tamaños de _tablaHash evaluados, esta función produce la menor media de colisiones, el mínimo de colisiones máximas por inserción 
y el menor número de situaciones críticas en las que se superan 10 colisiones, superando claramente a la función cuadrática 
y también a la otra función doble. Además, con λ = 0.65 se obtiene un tamaño de _tablaHash que reduce ligeramente las colisiones respecto 
a λ = 0.68, manteniendo un uso razonable de memoria. Por tanto, esta combinación es la que garantiza un rendimiento más consistente 
y óptimo tanto en inserciones como en las búsquedas masivas requeridas en la práctica.


## Comparación de tiempos dados en milisegundos
Tiempo en realizar las búsquedas en la Tabla Hash: 0 ms 
Tiempo en realizar las búsquedas en la Lista: 82 ms

Hemos confirmado que la Tabla Hash ofrece un rendimiento claramente superior frente a la lista enlazada para las operaciones de
búsqueda exigidas en la práctica. 
En las mediciones realizadas, la búsqueda en la Tabla Hash ha requerido 0 ms, 
mientras que la misma operación sobre la lista ha necesitado 82 ms, 
lo que supone una diferencia abrumadora.

Esta mejora se debe a que la Tabla Hash mantiene una complejidad promedio O(1), 
permitiendo localizar cualquier elemento mediante una única evaluación de la función hash y, 
en la mayoría de los casos, con muy pocos intentos adicionales gracias a un factor de carga adecuado 
y a un buen diseño en las funciones de dispersión. Por el contrario, la lista realiza una búsqueda secuencial con 
complejidad O(n), lo que obliga a recorrer elemento por elemento hasta hallar el identificador buscado.

Por tanto, la Tabla Hash no solo reduce drásticamente el tiempo de búsqueda, 
sino que ofrece un comportamiento estable y predecible incluso con un volumen masivo de consultas, justificando plenamente su uso 
como estructura principal en el sistema MediExpress.