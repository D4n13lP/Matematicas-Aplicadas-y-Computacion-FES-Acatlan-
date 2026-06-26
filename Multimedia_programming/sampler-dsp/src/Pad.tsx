interface ColorRamp {
  bg: string
  border: string
  text: string
  dot: string
}

export interface PadDef {
  id: number
  name: string
  key: string
  ramp: ColorRamp
}

interface Props {
  pad: PadDef
  active: boolean
  onTrigger: (id: number) => void
}

export default function Pad({ pad, active, onTrigger }: Props) {
  const { ramp } = pad
  return (
    <button
      onPointerDown={() => onTrigger(pad.id)}
      style={{
        aspectRatio: '1',
        borderRadius: '8px',
        border: `1px solid ${active ? ramp.border : '#2a2a2a'}`,
        background: active ? ramp.bg : '#191919',
        display: 'flex',
        flexDirection: 'column',
        alignItems: 'center',
        justifyContent: 'center',
        gap: '5px',
        cursor: 'pointer',
        transition: 'transform 0.07s ease, background 0.07s ease, border-color 0.07s ease',
        transform: active ? 'scale(0.92)' : 'scale(1)',
        userSelect: 'none',
        touchAction: 'manipulation',
      }}
    >
      <span
        style={{
          fontSize: '15px',
          fontWeight: 600,
          letterSpacing: '0.05em',
          color: active ? ramp.text : '#c8c8c8',
        }}
      >
        {pad.name}
      </span>
      <span
        style={{
          fontSize: '12px',
          color: active ? ramp.border : '#444',
        }}
      >
        [{pad.key}]
      </span>
      {active && (
        <div
          style={{
            width: '5px',
            height: '5px',
            borderRadius: '50%',
            background: ramp.dot,
          }}
        />
      )}
    </button>
  )
}
