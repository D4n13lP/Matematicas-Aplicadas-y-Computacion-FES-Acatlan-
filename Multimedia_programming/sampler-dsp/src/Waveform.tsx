import { useEffect, useRef } from 'react'

interface Props {
  analyser: AnalyserNode | null
  filterFreq: number
  playbackRate: number
}

export default function Waveform({ analyser, filterFreq, playbackRate }: Props) {
  const canvasRef = useRef<HTMLCanvasElement>(null)
  const rafRef = useRef<number>(0)

  useEffect(() => {
    const canvas = canvasRef.current
    if (!canvas) return
    const ctx = canvas.getContext('2d')!

    const draw = () => {
      rafRef.current = requestAnimationFrame(draw)
      const w = canvas.width
      const h = canvas.height

      ctx.clearRect(0, 0, w, h)

      // Grid lines
      ctx.strokeStyle = '#1e1e1e'
      ctx.lineWidth = 1
      for (let i = 1; i < 4; i++) {
        ctx.beginPath()
        ctx.moveTo(0, (h / 4) * i)
        ctx.lineTo(w, (h / 4) * i)
        ctx.stroke()
      }

      if (!analyser) {
        // Flat baseline when idle
        ctx.strokeStyle = '#2d2d2d'
        ctx.lineWidth = 1.5
        ctx.beginPath()
        ctx.moveTo(0, h / 2)
        ctx.lineTo(w, h / 2)
        ctx.stroke()
        return
      }

      const data = new Uint8Array(analyser.frequencyBinCount)
      analyser.getByteTimeDomainData(data)

      ctx.strokeStyle = '#7c73d8'
      ctx.lineWidth = 1.8
      ctx.beginPath()
      const sliceW = w / data.length
      for (let i = 0; i < data.length; i++) {
        const x = i * sliceW
        const y = (data[i] / 255) * h
        i === 0 ? ctx.moveTo(x, y) : ctx.lineTo(x, y)
      }
      ctx.stroke()
    }

    draw()
    return () => cancelAnimationFrame(rafRef.current)
  }, [analyser])

  const fStr =
    filterFreq >= 1000
      ? `${(filterFreq / 1000).toFixed(1)} kHz`
      : `${filterFreq} Hz`

  return (
    <div className="rounded-lg border border-zinc-800 bg-zinc-950 overflow-hidden mb-5">
      <div className="flex items-center justify-between px-3 py-2 border-b border-zinc-800">
        <span className="text-sm text-zinc-600 tracking-widest">WAVEFORM</span>
        <span className="text-sm text-zinc-500">
          {fStr} · {playbackRate.toFixed(2)}×
        </span>
      </div>
      <canvas
        ref={canvasRef}
        width={900}
        height={80}
        style={{ width: '100%', height: '80px', display: 'block' }}
      />
    </div>
  )
}
