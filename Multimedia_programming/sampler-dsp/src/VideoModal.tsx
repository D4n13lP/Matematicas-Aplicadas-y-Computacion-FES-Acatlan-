import { useEffect } from 'react'

interface Props {
  open: boolean
  onClose: () => void
  youtubeUrl: string
}

// ──────────────────────────────────────────────────────────────────────────
// Convierte casi cualquier formato de URL de YouTube (watch?v=, youtu.be/,
// shorts/, embed/) en una URL de embed válida. Si no logra reconocer el
// formato, regresa null y el modal muestra un aviso en vez de un iframe roto.
// ──────────────────────────────────────────────────────────────────────────
function toYoutubeEmbedUrl(url: string): string | null {
  if (!url) return null
  try {
    const u = new URL(url)
    let id: string | null = null

    if (u.hostname.includes('youtu.be')) {
      id = u.pathname.slice(1)
    } else if (u.hostname.includes('youtube.com')) {
      if (u.pathname === '/watch') {
        id = u.searchParams.get('v')
      } else if (u.pathname.startsWith('/embed/')) {
        id = u.pathname.split('/embed/')[1]
      } else if (u.pathname.startsWith('/shorts/')) {
        id = u.pathname.split('/shorts/')[1]
      }
    }

    if (!id) return null
    id = id.split('?')[0].split('&')[0]
    return `https://www.youtube.com/embed/${id}?autoplay=1&rel=0`
  } catch {
    return null
  }
}

export default function VideoModal({ open, onClose, youtubeUrl }: Props) {
  // Cerrar con la tecla Escape
  useEffect(() => {
    if (!open) return
    const onKey = (e: KeyboardEvent) => {
      if (e.key === 'Escape') onClose()
    }
    window.addEventListener('keydown', onKey)
    // Bloquear el scroll del fondo mientras el modal está abierto
    const prevOverflow = document.body.style.overflow
    document.body.style.overflow = 'hidden'
    return () => {
      window.removeEventListener('keydown', onKey)
      document.body.style.overflow = prevOverflow
    }
  }, [open, onClose])

  if (!open) return null

  const embedUrl = toYoutubeEmbedUrl(youtubeUrl)

  return (
    <div
      role="dialog"
      aria-modal="true"
      onClick={onClose}
      style={{
        position: 'fixed',
        inset: 0,
        zIndex: 1000,
        // Efecto "tipo cine": fondo casi negro con un leve desenfoque
        background: 'rgba(0, 0, 0, 0.92)',
        backdropFilter: 'blur(4px)',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        padding: '2rem',
        animation: 'dspModalFadeIn 0.2s ease-out',
      }}
    >
      <style>{`
        @keyframes dspModalFadeIn {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        @keyframes dspModalScaleIn {
          from { opacity: 0; transform: scale(0.96); }
          to   { opacity: 1; transform: scale(1); }
        }
      `}</style>

      <div
        onClick={(e) => e.stopPropagation()}
        style={{
          position: 'relative',
          width: '100%',
          maxWidth: '960px',
          animation: 'dspModalScaleIn 0.25s ease-out',
        }}
      >
        {/* Botón cerrar */}
        <button
          onClick={onClose}
          aria-label="Cerrar video"
          style={{
            position: 'absolute',
            top: '-44px',
            right: 0,
            background: 'transparent',
            border: '1px solid #3a3a3a',
            color: '#e8e8e8',
            width: '34px',
            height: '34px',
            borderRadius: '8px',
            cursor: 'pointer',
            fontSize: '18px',
            lineHeight: 1,
          }}
        >
          ✕
        </button>

        <div
          style={{
            width: '100%',
            aspectRatio: '16 / 9',
            background: '#000',
            borderRadius: '12px',
            overflow: 'hidden',
            boxShadow: '0 20px 60px rgba(0,0,0,0.6)',
            border: '1px solid #262626',
          }}
        >
          {embedUrl ? (
            <iframe
              src={embedUrl}
              title="Video del proyecto"
              allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
              allowFullScreen
              style={{ width: '100%', height: '100%', border: 'none' }}
            />
          ) : (
            <div
              style={{
                width: '100%',
                height: '100%',
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'center',
                color: '#666',
                fontSize: '14px',
                textAlign: 'center',
                padding: '1.5rem',
              }}
            >
              Aún no se ha configurado el enlace del video de YouTube.
            </div>
          )}
        </div>
      </div>
    </div>
  )
}
