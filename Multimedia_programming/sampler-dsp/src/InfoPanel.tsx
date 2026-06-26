export default function InfoPanel() {
  const paragraphs: string[] = [
    'Se dispara un sample de audio presionando cualquiera de los nueve pads. Cada pad produce un sonido distinto: Kick y Snare conforman la base rítmica, HH cerrado y HH abierto corresponden a platillos de batería, Clap genera un aplauso, Tom reproduce un tambor de piso, Bass lanza un golpe de bajo y Synth activa un acorde sintético. Los pads también se pueden activar desde el teclado usando las teclas Q W E R en la fila superior y A S D F en la inferior.',
    'El slider Filtro pasa-bajos determina qué tan brillante o apagado se escucha cada sample. Al desplazarlo hacia la izquierda se cortan las frecuencias agudas y el sonido se vuelve opaco, como si proviniera de debajo del agua. Al desplazarlo hacia la derecha se deja pasar todo el espectro y el sonido se escucha abierto y nítido.',
    'El slider Velocidad de reproducción controla qué tan rápido o lento se lee el audio. Al subirse por encima de 1× el sample se acelera y el tono sube, produciendo el efecto de voz de ardilla. Al bajarse por debajo de 1× el sample se ralentiza y el tono cae, produciendo el efecto de voz de monstruo. En 1× el sonido se reproduce a su velocidad y tono originales.',
    'El monitor de Waveform en la parte superior muestra la forma de onda de cada sample en el momento exacto en que se reproduce, reflejando en tiempo real el efecto combinado del filtro y la velocidad seleccionados.',
  ]

  return (
    <div
      style={{
        borderRadius: '12px',
        border: '1px solid #1e1e1e',
        background: '#111',
        padding: '1.25rem 1.5rem',
        height: '100%',
      }}
    >
      <p style={{ fontSize: '13px', color: '#a594f9', letterSpacing: '0.1em', margin: '0 0 1.1rem', fontWeight: 700 }}>
        CÓMO USAR ESTA APLICACIÓN
      </p>

      <div style={{ display: 'flex', flexDirection: 'column', gap: '1.1rem' }}>
        {paragraphs.map((text, i) => (
          <p key={i} style={{ margin: 0, fontSize: '15px', color: '#9a9a9a', lineHeight: 1.75 }}>
            {text}
          </p>
        ))}
      </div>

      <p
        style={{
          fontSize: '11px',
          color: '#2a2a2a',
          marginTop: '1.25rem',
          marginBottom: 0,
          letterSpacing: '0.06em',
          textAlign: 'center',
        }}
      >
        AudioBufferSourceNode → BiquadFilterNode → AnalyserNode → AudioDestinationNode
      </p>
    </div>
  )
}
