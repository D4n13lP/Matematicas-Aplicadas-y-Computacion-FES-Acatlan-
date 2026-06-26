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

### Opción B — GitHub Actions (automático en cada push)
1. Subir el código fuente completo del proyecto (no `dist/`) a GitHub
2. En GitHub → Settings → Pages → Source, elegir **GitHub Actions**
3. Usar un workflow estándar de Vite + GitHub Pages (build con `npm run build`
   y publicar el contenido de `dist/`); GitHub sugiere una plantilla lista
   para Vite al elegir esta opción

> Nota: si en algún momento cambias `base: './'` por una ruta fija como
> `base: '/nombre-repo/'`, asegúrate de que coincida exactamente con el nombre
> del repositorio, o las rutas de los assets (CSS, JS, `logo.gif`) no se
> resolverán correctamente.

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
