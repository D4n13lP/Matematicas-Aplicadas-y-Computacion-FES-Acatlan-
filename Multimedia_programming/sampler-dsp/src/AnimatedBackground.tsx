import { useEffect, useRef } from 'react'

// ──────────────────────────────────────────────────────────────────────────
//  Fondo animado de toda la aplicación.
//  Dibuja, sobre un canvas a pantalla completa y en posición fija:
//   · puntos brillantes con una cola que los sigue (efecto "cometa")
//   · cubos en wireframe que flotan y rotan lentamente
//  Ambos en varios colores, sin ninguna referencia a logos ni marcas.
//  Vive en z-index bajo, detrás de todo el contenido, y no intercepta clics.
// ──────────────────────────────────────────────────────────────────────────

const COLORS = [
  '#42c4e7', // celeste
  '#7e42e7', // morado
  '#5DCAA5', // verde agua
  '#EF9F27', // ámbar
  '#F08FC0', // rosa
  '#534AB7', // azul violeta
]

interface Comet {
  x: number
  y: number
  vx: number
  vy: number
  radius: number
  color: string
  trail: { x: number; y: number }[]
}

interface Cube {
  x: number
  y: number
  size: number
  rotX: number
  rotY: number
  speedX: number
  speedY: number
  driftX: number
  driftY: number
  driftPhase: number
  color: string
}

function makeComet(w: number, h: number): Comet {
  const angle = Math.random() * Math.PI * 2
  const speed = 0.4 + Math.random() * 0.6
  return {
    x: Math.random() * w,
    y: Math.random() * h,
    vx: Math.cos(angle) * speed,
    vy: Math.sin(angle) * speed,
    radius: 1.6 + Math.random() * 1.8,
    color: COLORS[Math.floor(Math.random() * COLORS.length)],
    trail: [],
  }
}

function makeCube(w: number, h: number): Cube {
  return {
    x: Math.random() * w,
    y: Math.random() * h,
    size: 14 + Math.random() * 22,
    rotX: Math.random() * Math.PI * 2,
    rotY: Math.random() * Math.PI * 2,
    speedX: (Math.random() - 0.5) * 0.01,
    speedY: (Math.random() - 0.5) * 0.01,
    driftX: 8 + Math.random() * 10,
    driftY: 6 + Math.random() * 8,
    driftPhase: Math.random() * Math.PI * 2,
    color: COLORS[Math.floor(Math.random() * COLORS.length)],
  }
}

// Proyección isométrica simple de un cubo en wireframe, rotando en X e Y.
function projectCube(cube: Cube, t: number): [number, number][] {
  const s = cube.size
  const verts: [number, number, number][] = [
    [-s, -s, -s], [s, -s, -s], [s, s, -s], [-s, s, -s],
    [-s, -s, s], [s, -s, s], [s, s, s], [-s, s, s],
  ]
  const rx = cube.rotX + t * cube.speedX
  const ry = cube.rotY + t * cube.speedY
  const cosX = Math.cos(rx), sinX = Math.sin(rx)
  const cosY = Math.cos(ry), sinY = Math.sin(ry)

  const driftX = Math.sin(t * 0.0006 + cube.driftPhase) * cube.driftX
  const driftY = Math.cos(t * 0.0005 + cube.driftPhase) * cube.driftY

  return verts.map(([vx, vy, vz]) => {
    // Rotación en Y
    const x1 = vx * cosY - vz * sinY
    const z1 = vx * sinY + vz * cosY
    // Rotación en X
    const y2 = vy * cosX - z1 * sinX
    // Proyección simple (perspectiva leve)
    const scale = 1 + z1 * 0.0015
    return [cube.x + x1 * scale + driftX, cube.y + y2 * scale + driftY]
  })
}

const CUBE_EDGES: [number, number][] = [
  [0, 1], [1, 2], [2, 3], [3, 0],
  [4, 5], [5, 6], [6, 7], [7, 4],
  [0, 4], [1, 5], [2, 6], [3, 7],
]

export default function AnimatedBackground() {
  const canvasRef = useRef<HTMLCanvasElement | null>(null)

  useEffect(() => {
    const canvas = canvasRef.current
    if (!canvas) return
    const ctx = canvas.getContext('2d')
    if (!ctx) return

    let w = window.innerWidth
    let h = window.innerHeight
    let dpr = Math.min(window.devicePixelRatio || 1, 2)

    const resize = () => {
      w = window.innerWidth
      h = window.innerHeight
      dpr = Math.min(window.devicePixelRatio || 1, 2)
      canvas.width = w * dpr
      canvas.height = h * dpr
      canvas.style.width = `${w}px`
      canvas.style.height = `${h}px`
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0)
    }
    resize()
    window.addEventListener('resize', resize)

    // Menos elementos en pantallas chicas, para no saturar ni pesar de más
    const isSmall = w < 768
    const comets: Comet[] = Array.from(
      { length: isSmall ? 5 : 9 },
      () => makeComet(w, h)
    )
    const cubes: Cube[] = Array.from(
      { length: isSmall ? 4 : 7 },
      () => makeCube(w, h)
    )

    let rafId = 0
    let running = true

    const tick = (t: number) => {
      if (!running) return
      ctx.clearRect(0, 0, w, h)

      // ── Cubos en wireframe, flotando y rotando ──
      for (const cube of cubes) {
        const pts = projectCube(cube, t)
        ctx.strokeStyle = cube.color
        ctx.globalAlpha = 0.22
        ctx.lineWidth = 1
        ctx.beginPath()
        for (const [a, b] of CUBE_EDGES) {
          ctx.moveTo(pts[a][0], pts[a][1])
          ctx.lineTo(pts[b][0], pts[b][1])
        }
        ctx.stroke()
      }
      ctx.globalAlpha = 1

      // ── Cometas: punto brillante con cola ──
      for (const comet of comets) {
        comet.x += comet.vx
        comet.y += comet.vy

        // Rebote suave en los bordes de la pantalla
        if (comet.x < 0 || comet.x > w) comet.vx *= -1
        if (comet.y < 0 || comet.y > h) comet.vy *= -1
        comet.x = Math.max(0, Math.min(w, comet.x))
        comet.y = Math.max(0, Math.min(h, comet.y))

        comet.trail.push({ x: comet.x, y: comet.y })
        if (comet.trail.length > 18) comet.trail.shift()

        // Cola: línea que se va desvaneciendo hacia atrás
        for (let i = 1; i < comet.trail.length; i++) {
          const p0 = comet.trail[i - 1]
          const p1 = comet.trail[i]
          const alpha = (i / comet.trail.length) * 0.5
          ctx.strokeStyle = comet.color
          ctx.globalAlpha = alpha
          ctx.lineWidth = comet.radius * 1.1
          ctx.lineCap = 'round'
          ctx.beginPath()
          ctx.moveTo(p0.x, p0.y)
          ctx.lineTo(p1.x, p1.y)
          ctx.stroke()
        }

        // Punto brillante (con halo)
        ctx.globalAlpha = 1
        const glow = ctx.createRadialGradient(
          comet.x, comet.y, 0,
          comet.x, comet.y, comet.radius * 5
        )
        glow.addColorStop(0, comet.color)
        glow.addColorStop(1, 'rgba(0,0,0,0)')
        ctx.fillStyle = glow
        ctx.beginPath()
        ctx.arc(comet.x, comet.y, comet.radius * 5, 0, Math.PI * 2)
        ctx.fill()

        ctx.fillStyle = comet.color
        ctx.beginPath()
        ctx.arc(comet.x, comet.y, comet.radius, 0, Math.PI * 2)
        ctx.fill()
      }
      ctx.globalAlpha = 1

      rafId = requestAnimationFrame(tick)
    }

    rafId = requestAnimationFrame(tick)

    return () => {
      running = false
      cancelAnimationFrame(rafId)
      window.removeEventListener('resize', resize)
    }
  }, [])

  return (
    <canvas
      ref={canvasRef}
      aria-hidden="true"
      style={{
        position: 'fixed',
        inset: 0,
        zIndex: 0,
        pointerEvents: 'none',
        display: 'block',
      }}
    />
  )
}
