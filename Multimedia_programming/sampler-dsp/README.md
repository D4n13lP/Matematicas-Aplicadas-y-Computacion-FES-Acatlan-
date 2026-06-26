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
- Todos los samples son **sintetizados en memoria** (sin archivos externos)
