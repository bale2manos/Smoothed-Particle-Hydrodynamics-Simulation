# Simulación de Fluidos Orientado al Rendimiento

[![Estado del Proyecto](https://img.shields.io/badge/estado-finalizado-red.svg)](https://github.com/bale2manos/ac_p1/tree/final_version)

Enlace al [Repositorio en GitHub](https://github.com/bale2manos/ac_p1/tree/final_version).

Descripción concisa de la simulación de fluidos orientada al rendimiento.

## Contenido

- [Compilación en Avignon](#compilación-en-avignon)
- [Ejecución con 1000 Iteraciones](#ejecución-con-1000-iteraciones)
- [Rendimiento del Programa](#rendimiento-del-programa)
- [Consumo de Energía](#consumo-de-energía)
- [Cachegrind de Valgrind](#cachegrind-de-valgrind)
- [Tests Unitarios](#tests-unitarios)
- [Tests Funcionales](#tests-funcionales)
- [Documentación Doxygen](#documentación-doxygen)

## Compilación en Avignon

Para compilar el proyecto en Avignon, ejecuta el siguiente comando:

```bash
sbatch -o compilation_result.txt ./scripts/compilation.sh
```

## Ejecución con 1000 Iteraciones

Para ejecutar el programa con el archivo `large.fld` y 1000 iteraciones, utiliza el siguiente comando:

```bash
sbatch -o mil_iteraciones.txt ./scripts/mil_iterations.sh
```

## Rendimiento del Programa

Para observar el rendimiento del programa usando `perf stat -r 5`, ejecuta el siguiente comando:

```bash
sbatch -o perf_stat.txt ./scripts/perf_stat.sh
```
## Consumo de Energía

Para observar la energía consumida por el programa, utiliza el siguiente comando:

```bash
sbatch -o perf_stat_energy.txt ./scripts/perf_stat_energy.sh
```

## Cachegrind de Valgrind

Para observar los resultados del cachegrind de Valgrind, ejecuta el siguiente comando:

```bash
sbatch -o cachegrind.txt ./scripts/cachegrind.sh
```

## Tests Unitarios
Para ejecutar los tests unitarios, utiliza el siguiente comando:

```bash
sbatch -o unit_tests.txt ./ftest/testutest.sh
```

## Tests Funcionales
Para ejecutar los tests funcionales, ejecuta los scripts dentro de la carpeta "ftest", como por ejemplo:

```bash
sbatch -o functional_tests.txt ./ftest/ftest1debug10.sh
```

## Documentación Doxygen
La documentación Doxygen del proyecto se encuentra en la carpeta "doxygen-documentation". Consulta esta carpeta para obtener información detallada sobre la documentación.
