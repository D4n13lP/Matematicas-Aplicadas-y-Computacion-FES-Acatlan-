import { useState, useRef, useEffect, useCallback } from 'react'
import Waveform from './Waveform'
import Pad, { PadDef } from './Pad'
import InfoPanel from './InfoPanel'
import Logo from './Logo'
import BuildStory from './BuildStory'
import AnimatedBackground from './AnimatedBackground'
import {
  genKick, genSnare, genHHclosed, genHHopen,
  genClap, genTom, genBass, genSynth, loadVozSample,
} from './sounds'

// ── Definición de pads ─────────────────────────────────────────────────────
// Los primeros 8 pads se sintetizan en memoria (osciladores/ruido).
// El noveno pad ("Voz") reproduce un sample de audio real cargado/decodificado.

const SYNTH_PADS = [
  {
    id: 0, name: 'Kick', key: 'Q', gen: genKick,
    ramp: { bg: '#2a1a14', border: '#993C1D', text: '#F0997B', dot: '#F0997B' },
  },
  {
    id: 1, name: 'Snare', key: 'W', gen: genSnare,
    ramp: { bg: '#2a1a14', border: '#993C1D', text: '#F0997B', dot: '#F0997B' },
  },
  {
    id: 2, name: 'HH cerr.', key: 'E', gen: genHHclosed,
    ramp: { bg: '#271f0e', border: '#854F0B', text: '#EF9F27', dot: '#EF9F27' },
  },
  {
    id: 3, name: 'HH abi.', key: 'R', gen: genHHopen,
    ramp: { bg: '#271f0e', border: '#854F0B', text: '#EF9F27', dot: '#EF9F27' },
  },
  {
    id: 4, name: 'Clap', key: 'A', gen: genClap,
    ramp: { bg: '#0d2018', border: '#0F6E56', text: '#5DCAA5', dot: '#5DCAA5' },
  },
  {
    id: 5, name: 'Tom', key: 'S', gen: genTom,
    ramp: { bg: '#0d2018', border: '#0F6E56', text: '#5DCAA5', dot: '#5DCAA5' },
  },
  {
    id: 6, name: 'Bass', key: 'D', gen: genBass,
    ramp: { bg: '#15132a', border: '#534AB7', text: '#AFA9EC', dot: '#AFA9EC' },
  },
  {
    id: 7, name: 'Synth', key: 'F', gen: genSynth,
    ramp: { bg: '#15132a', border: '#534AB7', text: '#AFA9EC', dot: '#AFA9EC' },
  },
] as (PadDef & { gen: (ctx: AudioContext) => AudioBuffer })[]

type PadWithGen = PadDef & { gen: (ctx: AudioContext) => AudioBuffer }
const SYNTH_PADS_WITH_GEN = SYNTH_PADS as PadWithGen[]

// Noveno pad: sample de voz real (no sintetizado), id 8, tecla G
const VOZ_PAD: PadDef = {
  id: 8, name: 'Voz', key: 'G',
  ramp: { bg: '#2a0d1f', border: '#C23B7A', text: '#F08FC0', dot: '#F08FC0' },
}

const PADS: PadDef[] = [...SYNTH_PADS_WITH_GEN, VOZ_PAD]

const KEY_MAP: Record<string, number> = { q:0, w:1, e:2, r:3, a:4, s:5, d:6, f:7, g:8 }

// ── Componente principal ───────────────────────────────────────────────────

export default function App() {
  const [filterFreq, setFilterFreq] = useState(18000)
  const [playbackRate, setPlaybackRate] = useState(1)
  const [activePads, setActivePads] = useState<Set<number>>(new Set())
  const [isReady, setIsReady] = useState(false)
  const [analyser, setAnalyser] = useState<AnalyserNode | null>(null)

  const ctxRef = useRef<AudioContext | null>(null)
  const bufsRef = useRef<AudioBuffer[]>([])
  const filterRef = useRef<BiquadFilterNode | null>(null)
  const prRef = useRef(1)               // ref para evitar stale closure en triggerPad

  // Mantener prRef sincronizado
  useEffect(() => { prRef.current = playbackRate }, [playbackRate])

  // Actualizar frecuencia del filtro en tiempo real
  useEffect(() => {
    if (filterRef.current) filterRef.current.frequency.value = filterFreq
  }, [filterFreq])

  // ── Inicializar AudioContext (requiere gesto del usuario) ──────────────
  const initAudio = useCallback((): AudioContext => {
    if (ctxRef.current) {
      void ctxRef.current.resume()
      return ctxRef.current
    }

    const ctx = new AudioContext()

    // Nodo 2: Analizador (para visualización de waveform)
    const an = ctx.createAnalyser()
    an.fftSize = 1024

    // Nodo 1: Filtro pasa-bajos (BiquadFilterNode)
    const filter = ctx.createBiquadFilter()
    filter.type = 'lowpass'
    filter.frequency.value = 18000
    filter.Q.value = 0.7

    // Grafo: fuente → filtro → analizador → destino (bocinas)
    filter.connect(an)
    an.connect(ctx.destination)

    filterRef.current = filter
    ctxRef.current = ctx

    // Sintetizar los 8 buffers en memoria (síntesis matemática)
    const buffers: AudioBuffer[] = SYNTH_PADS_WITH_GEN.map((p) => p.gen(ctx))
    bufsRef.current = buffers

    setAnalyser(an)
    setIsReady(true)

    // Cargar/decodificar el sample de voz real en paralelo (no bloquea los demás pads)
    loadVozSample(ctx)
      .then((buf) => { bufsRef.current[8] = buf })
      .catch((err) => console.error('No se pudo decodificar el sample de voz:', err))

    return ctx
  }, [])

  // ── Disparar un sample ─────────────────────────────────────────────────
  const triggerPad = useCallback((id: number) => {
    const ctx = initAudio()
    if (!filterRef.current || !bufsRef.current[id]) return

    // Nodo 0: Fuente (AudioBufferSourceNode) — cero latencia
    const src = ctx.createBufferSource()
    src.buffer = bufsRef.current[id]
    src.playbackRate.value = prRef.current   // Efecto ardilla / monstruo
    src.connect(filterRef.current)
    src.start()

    // Feedback visual del pad
    setActivePads((prev) => new Set([...prev, id]))
    setTimeout(() => {
      setActivePads((prev) => {
        const next = new Set(prev)
        next.delete(id)
        return next
      })
    }, 180)
  }, [initAudio])

  // ── Soporte de teclado ─────────────────────────────────────────────────
  useEffect(() => {
    const onKey = (e: KeyboardEvent) => {
      if (e.repeat) return
      const id = KEY_MAP[e.key.toLowerCase()]
      if (id !== undefined) triggerPad(id)
    }
    window.addEventListener('keydown', onKey)
    return () => window.removeEventListener('keydown', onKey)
  }, [triggerPad])

  const fStr =
    filterFreq >= 1000
      ? `${(filterFreq / 1000).toFixed(1)} kHz`
      : `${filterFreq} Hz`

  return (
    <div className="min-h-screen bg-[#0f0f0f] relative">

      {/* ── Fondo animado: cometas con cola y cubos flotando ── */}
      <AnimatedBackground />

      {/* ── Encabezado (ancho completo, con imagen de ondas de sonido de fondo) ── */}
      <header className="font-heading relative z-10 w-full border-b border-zinc-800 px-4 sm:px-8 py-5 overflow-hidden">
        {/* Imagen de fondo del encabezado, atenuada para no competir con el texto */}
        <div
          aria-hidden="true"
          className="absolute inset-0 -z-10"
          style={{
            backgroundImage: `url(${import.meta.env.BASE_URL}header-bg.jpg)`,
            backgroundSize: 'cover',
            backgroundPosition: 'center',
            opacity: 0.28,
          }}
        />
        {/* Degradado para apagar aún más la imagen cerca del texto y unificarla con el fondo oscuro */}
        <div
          aria-hidden="true"
          className="absolute inset-0 -z-10"
          style={{
            background:
              'linear-gradient(90deg, rgba(15,15,15,0.85) 0%, rgba(15,15,15,0.55) 45%, rgba(15,15,15,0.85) 100%)',
          }}
        />

        <div className="flex items-center gap-5">
          <Logo size={88} />
          <div className="flex flex-col gap-1 min-w-0">
            <h1 className="text-2xl sm:text-3xl lg:text-4xl font-bold tracking-wide text-zinc-100 leading-tight">
              Digital Signal Processing
            </h1>
            <p className="text-sm sm:text-base text-zinc-400 tracking-widest">
              Programación Multimedia
            </p>
            <p className="text-sm sm:text-base text-zinc-500 tracking-wide">
              Daniel Pineda Ortega
            </p>
          </div>
          <div className="flex-1" />
          <span
            className="text-xs sm:text-sm tracking-widest whitespace-nowrap"
            style={{ color: isReady ? '#5DCAA5' : '#444' }}
          >
            {isReady ? '● LIVE' : '○ IDLE'}
          </span>
        </div>
      </header>

      {/* ── Contenido ── */}
      <main className="relative z-10 px-4 sm:px-8 py-6 max-w-[1400px] mx-auto">

        {/* ── Monitor de waveform (siempre arriba, ancho completo del contenido) ── */}
        <Waveform
          analyser={analyser}
          filterFreq={filterFreq}
          playbackRate={playbackRate}
        />

        {/*
          Vistas responsivas:
          · Móvil   (< md):  columna única — pads, controles, texto
          · Tablet  (md-xl): pads + controles centrados en una fila, texto debajo
          · Desktop (>= xl): texto | pads | controles en una sola fila
        */}
        <div className="flex flex-col xl:flex-row xl:items-stretch gap-5 xl:gap-6">

          {/* Texto explicativo — a la izquierda en desktop, al final en móvil/tablet */}
          <div className="order-3 xl:order-1 xl:w-[320px] xl:flex-shrink-0">
            <InfoPanel />
          </div>

          {/* Pads + controles (+ GIF de relleno en desktop) — agrupados, centrados en tablet/desktop */}
          <div className="order-1 xl:order-2 flex flex-col gap-5 flex-1">

            <div className="flex flex-col md:flex-row md:items-start md:justify-center gap-5">
              {/* Grilla de pads (3 x 3) */}
              <div className="grid grid-cols-3 gap-3 w-full md:w-[340px] md:flex-shrink-0">
                {PADS.map((pad) => (
                  <Pad
                    key={pad.id}
                    pad={pad}
                    active={activePads.has(pad.id)}
                    onTrigger={triggerPad}
                  />
                ))}
              </div>

              {/* Controles DSP */}
              <div className="rounded-xl border border-zinc-800 bg-zinc-900 p-5 space-y-5 w-full md:w-[320px] md:flex-shrink-0">
                <p className="text-sm text-zinc-600 tracking-widest mb-1">DSP CONTROLS</p>

                {/* Filtro pasa-bajos */}
                <div>
                  <div className="flex justify-between mb-2">
                    <label className="text-sm text-zinc-400 tracking-wide">
                      Filtro pasa-bajos <span className="text-zinc-600">(BiquadFilterNode)</span>
                    </label>
                    <span className="text-sm font-semibold" style={{ color: '#7c73d8' }}>
                      {fStr}
                    </span>
                  </div>
                  <input
                    type="range"
                    min={200}
                    max={20000}
                    step={100}
                    value={filterFreq}
                    onChange={(e) => setFilterFreq(Number(e.target.value))}
                  />
                  <div className="flex justify-between mt-1">
                    <span className="text-xs text-zinc-700">ahogado</span>
                    <span className="text-xs text-zinc-700">abierto</span>
                  </div>
                </div>

                {/* Velocidad de reproducción */}
                <div>
                  <div className="flex justify-between mb-2">
                    <label className="text-sm text-zinc-400 tracking-wide">
                      Velocidad de reproducción <span className="text-zinc-600">(playbackRate)</span>
                    </label>
                    <span className="text-sm font-semibold" style={{ color: '#5DCAA5' }}>
                      {playbackRate.toFixed(2)}×
                    </span>
                  </div>
                  <input
                    type="range"
                    min={0.25}
                    max={4}
                    step={0.05}
                    value={playbackRate}
                    onChange={(e) => setPlaybackRate(Number(e.target.value))}
                  />
                  <div className="flex justify-between mt-1">
                    <span className="text-xs text-zinc-700">monstruo 0.25×</span>
                    <span className="text-xs text-zinc-700">ardilla 4×</span>
                  </div>
                </div>
              </div>
            </div>

            {/*
              GIF del logo animado — visible en los tres tamaños responsivos,
              pegado justo debajo de los pads/controles (sin centrado vertical
              que generara espacio vacío arriba). En desktop (xl) aprovecha el
              espacio restante de la columna gracias a flex-1; en móvil/tablet
              se ajusta a su tamaño natural respetando el ancho disponible.
            */}
            <div className="flex justify-center xl:flex-1 xl:min-h-0 xl:items-start">
              <img
                src={`${import.meta.env.BASE_URL}logo.gif`}
                alt="Logo animado DSP AudioPad"
                className="max-w-full xl:max-h-full w-auto h-auto object-contain"
              />
            </div>
          </div>
        </div>

        {/* Instrucciones */}
        {!isReady && (
          <p className="text-center text-sm text-zinc-700 tracking-wider mt-5">
            Presiona un pad o tecla [Q W E R / A S D F G] para activar el audio
          </p>
        )}

        {/* ── Cómo se construyó este proyecto (video + repositorio) ── */}
        <BuildStory />
      </main>
    </div>
  )
}
