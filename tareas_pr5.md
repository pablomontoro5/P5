# Flujo Git y Reparto de Fases (Tú y Lázaro)

## 🧩 Repositorio y configuración inicial

### Crear repositorio local

``` bash
git init
git add .
git commit -m "Initial commit: estructura base del proyecto"
```

### Crear repositorio remoto y enlazarlo

``` bash
git remote add origin https://github.com/TU_USUARIO/practica5-hash.git
git branch -M main
git push -u origin main
```

### Añadir a tu compañero (en GitHub)

Settings → Collaborators → Add: Lázaro

------------------------------------------------------------------------

# 🟦 TÚ --- Responsable del hash, análisis y búsquedas

## Fase 1 --- Implementación básica de THashMedicam

``` bash
git checkout -b feature/thash-basico
# Trabajas en el código
git add .
git commit -m "Implementación base de _tablaHash hash"
git push -u origin feature/thash-basico
```

## Fase 2A --- Función hash cuadrática

``` bash
git checkout -b feature/hash-cuadratica
git add .
git commit -m "Función de exploración cuadrática implementada"
git push -u origin feature/hash-cuadratica
```

## Fase 3 --- Métricas

``` bash
git checkout -b feature/metricas-hash
git add .
git commit -m "Métricas de colisiones incluidas"
git push -u origin feature/metricas-hash
```

## Fase 4 --- Análisis de configuraciones

``` bash
git checkout -b feature/analisis-configuraciones
git add analisis_Thash.md
git commit -m "Resultados y análisis de configuraciones del hash"
git push -u origin feature/analisis-configuraciones
```

## Fase 5A --- Búsquedas por nombre

``` bash
git checkout -b feature/busqueda-nombre
git add .
git commit -m "Implementación de búsqueda por nombre con multimap"
git push -u origin feature/busqueda-nombre
```

## Fase 6A --- Programa de prueba 2 (parte de medicamentos)

``` bash
git checkout -b feature/pruebas-medicamentos
git add .
git commit -m "Búsquedas específicas y eliminación de compuestos completadas"
git push -u origin feature/pruebas-medicamentos
```

------------------------------------------------------------------------

# 🟩 LÁZARO --- Responsable del rendimiento y MediExpress

## Fase 2B --- Doble dispersión A y B

``` bash
git checkout -b feature/hash-dobleA
git add .
git commit -m "Doble dispersión A implementada"
git push -u origin feature/hash-dobleA
```

``` bash
git checkout -b feature/hash-dobleB
git add .
git commit -m "Doble dispersión B implementada"
git push -u origin feature/hash-dobleB
```

## Fase 3B --- Tamaño primo según λ

``` bash
git checkout -b feature/tamanio-primo
git add .
git commit -m "Cálculo automático de primos para tamaños de _tablaHash"
git push -u origin feature/tamanio-primo
```

## Fase 5B --- Integración MediExpress

``` bash
git checkout -b feature/integracion-mediexpress
git add .
git commit -m "Carga de ficheros y enlaces de contenedores completados"
git push -u origin feature/integracion-mediexpress
```

## Fase 6B --- Prueba de rendimiento

``` bash
git checkout -b feature/rendimiento
git add .
git commit -m "Comparación de tiempos entre hash y lista implementada"
git push -u origin feature/rendimiento
```

------------------------------------------------------------------------

# 🟨 Extra (Parejas) --- Redisperción dinámica (Lázaro)

``` bash
git checkout -b feature/redispersion
git add .
git commit -m "Redispersión dinámica incluida en insertarEnTabla()"
git push -u origin feature/redispersion
```

------------------------------------------------------------------------

# 🟪 Integración final

## Fusionar cada feature en develop

``` bash
git checkout develop
git merge feature/NOMBRE
git push
```

## Crear release final

``` bash
git checkout -b release/v1.0
git add .
git commit -m "Versión final lista para entrega"
git push -u origin release/v1.0
```

## Pasar a main

``` bash
git checkout main
git merge release/v1.0
git push
git tag v1.0
git push --tags
```

------------------------------------------------------------------------

# ✔️ Resumen de responsabilidades

  Fase                              Responsable
  --------------------------------- -------------
  THash básico                      Tú
  Hash cuadrática                   Tú
  Doble dispersión A y B            Lázaro
  Métricas                          Tú
  Tamaño primo                      Lázaro
  Análisis final                    Tú
  Carga de ficheros y MediExpress   Lázaro
  Multimap nombres                  Tú
  Rendimiento lista vs hash         Lázaro
  Redisperción dinámica             Lázaro
