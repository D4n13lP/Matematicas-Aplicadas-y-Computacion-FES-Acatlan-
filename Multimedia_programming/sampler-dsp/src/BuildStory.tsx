import { useState } from 'react'
import VideoModal from './VideoModal'
import { YOUTUBE_VIDEO_URL, GITHUB_REPO_URL } from './projectLinks'

export default function BuildStory() {
  const [videoOpen, setVideoOpen] = useState(false)

  const hasVideo = YOUTUBE_VIDEO_URL.trim().length > 0
  const hasRepo = GITHUB_REPO_URL.trim().length > 0

  return (
    <section
      style={{
        marginTop: '1.5rem',
        borderRadius: '12px',
        border: '1px solid #1e1e1e',
        background: '#111',
        padding: '1.5rem 1.75rem',
      }}
    >
      <h2
        style={{
        fontSize: '15px',
          letterSpacing: '0.1em',
          margin: '0 0 1rem',
          color: '#a594f9',
          fontWeight: 700,
        }}
      >
        CÓMO SE CONSTRUYÓ ESTE PROYECTO
      </h2>

      <p style={{ margin: '0 0 1.25rem', fontSize: '15px', color: '#9a9a9a', lineHeight: 1.7 }}>
        Esta sección reúne el material audiovisual y el código fuente del proyecto.
        Presiona el botón para ver el video explicativo subido a YouTube, y revisa
        el repositorio de GitHub si quieres explorar el código completo.
      </p>

      <div style={{ display: 'flex', flexDirection: 'column', gap: '0.6rem', alignItems: 'flex-start' }}>
        <button
          onClick={() => hasVideo && setVideoOpen(true)}
          disabled={!hasVideo}
          style={{
          fontSize: '14px',
            fontWeight: 700,
            letterSpacing: '0.05em',
            color: hasVideo ? '#0f0f0f' : '#666',
            background: hasVideo ? '#a594f9' : '#222',
            border: 'none',
            borderRadius: '8px',
            padding: '0.7rem 1.4rem',
            cursor: hasVideo ? 'pointer' : 'not-allowed',
            display: 'inline-flex',
            alignItems: 'center',
            gap: '0.5rem',
            transition: 'transform 0.1s ease, background 0.15s ease',
          }}
          title={hasVideo ? 'Ver el video del proyecto' : 'Aún no se ha configurado el enlace de YouTube'}
        >
          ▶ Ver video del proyecto
        </button>

        {hasRepo ? (
          <a
            href={GITHUB_REPO_URL}
            target="_blank"
            rel="noopener noreferrer"
            style={{
            fontSize: '13px',
              color: '#7c73d8',
              textDecoration: 'none',
              letterSpacing: '0.03em',
            }}
          >
            ↗ Ver repositorio en GitHub
          </a>
        ) : (
          <span
            style={{
            fontSize: '13px',
              color: '#555',
              letterSpacing: '0.03em',
            }}
          >
            ↗ Repositorio de GitHub (pendiente de configurar)
          </span>
        )}
      </div>

      <VideoModal
        open={videoOpen}
        onClose={() => setVideoOpen(false)}
        youtubeUrl={YOUTUBE_VIDEO_URL}
      />
    </section>
  )
}
