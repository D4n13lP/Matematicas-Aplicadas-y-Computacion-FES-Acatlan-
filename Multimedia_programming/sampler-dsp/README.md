# Sampler DSP

Aplicación de procesamiento de audio en tiempo real usando la Web Audio API.

## Instalación y uso local

```bash
# 1. Instalar dependencias
npm install

# 2. Correr en modo desarrollo
npm run dev
```

Abrir http://localhost:5173 en el navegador.

## Build para producción

```bash
npm run build
```

Genera la carpeta `dist/` lista para subir a Netlify.

## Deploy en Netlify

### Opción A — Drag & Drop (más fácil)
1. Correr `npm run build`
2. Ir a https://app.netlify.com/drop
3. Arrastrar la carpeta `dist/` al área de drop

### Opción B — GitHub + Netlify CI
1. Subir el proyecto a un repositorio de GitHub
2. Conectar el repo en https://app.netlify.com
3. Configuración automática (el archivo `netlify.toml` ya está incluido):
   - Build command: `npm run build`
   - Publish directory: `dist`

## Deploy en GitHub Pages

El proyecto ya está configurado con `base: './'` en `vite.config.ts`, así que el
build funciona tanto en la raíz de un dominio (Netlify) como en una subcarpeta
(`https://usuario.github.io/nombre-repo/`), que es como GitHub Pages publica los
repos normales.

### Opción A — Subir la carpeta `dist/` directamente (más simple)
1. Correr `npm run build`
2. Crear/usar un repositorio en GitHub y subir el **contenido** de `dist/`
   (no la carpeta completa, sino sus archivos) a la rama `main` (o a una rama
   `gh-pages`)
3. En GitHub → Settings → Pages → Source, elegir esa rama y la carpeta raíz `/`
4. Esperar uno o dos minutos y abrir la URL que GitHub Pages indique

### Opción B — GitHub Actions (automático en cada push, recomendado)

Este proyecto ya incluye el workflow `.github/workflows/deploy.yml`, listo para
usarse. Solo falta activarlo una vez desde la configuración del repositorio:

1. Subir el código fuente completo del proyecto (no `dist/`) a la rama `main`
   del repositorio
2. En GitHub → **Settings → Pages → Source**, elegir **GitHub Actions**
   (en vez de "Deploy from a branch")
3. Hacer cualquier push a `main` (o ir a la pestaña **Actions** del repo y
   ejecutar el workflow manualmente con "Run workflow")
4. Esperar a que el workflow termine (ícono verde ✓) y abrir la URL que
   aparece en Settings → Pages

> ⚠️ **Si tu proyecto vive en una subcarpeta del repositorio** (por ejemplo
> `Multimedia_programming/sampler-dsp/`, en vez de la raíz del repo), edita las
> tres líneas de `working-directory` y la línea `path:` dentro de
> `.github/workflows/deploy.yml` para que apunten a esa ruta exacta. El
> workflow incluido ya está configurado para `Multimedia_programming/sampler-dsp`;
> si moviste o renombraste la carpeta, actualiza esas rutas.

> Nota: el repositorio puede tener cualquier nombre — `base: './'` en
> `vite.config.ts` usa rutas relativas, así que **no es necesario** ajustarlo
> según el nombre del repo ni cuántos niveles de subcarpeta tenga la URL final
> de GitHub Pages.

## Controles

| Tecla | Pad      |
|-------|----------|
| Q     | Kick     |
| W     | Snare    |
| E     | HH cerr. |
| R     | HH abi.  |
| A     | Clap     |
| S     | Tom      |
| D     | Bass     |
| F     | Synth    |
| G     | Voz      |

## Grafo de Audio (AudioGraph)

```
AudioBufferSourceNode
        ↓
  BiquadFilterNode  ←── slider "Filtro pasa-bajos"
        ↓
   AnalyserNode     ←── waveform monitor (canvas)
        ↓
AudioDestinationNode (bocinas)
```

- **playbackRate** del BufferSource: controlado por slider "Velocidad de reproducción"
- 8 de los 9 pads son **sintetizados en memoria** (osciladores y ruido, sin archivos externos)
- El pad **Voz** reproduce un sample de audio real, decodificado desde un data URI embebido en el código
