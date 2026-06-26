// ──────────────────────────────────────────────
//  Logo animado del header (SVG + CSS keyframes)
//  Réplica fiel del diseño original (Penpot export):
//  triángulo con borde degradado, base amarilla,
//  rótulo "DSP AudioPad", planeta con anillo que
//  flota y una esfera gris que orbita alrededor.
// ──────────────────────────────────────────────
export default function Logo({ size = 56 }: { size?: number }) {
  return (
    <svg
      width={size}
      height={size * (264 / 268.003)}
      viewBox="-13 -16 268.003 264"
      xmlns="http://www.w3.org/2000/svg"
      style={{ flexShrink: 0, overflow: 'visible' }}
    >
      <style>{`
        @import url('https://fonts.googleapis.com/css2?family=Agbalumo&display=block');

        @keyframes dspLogoOrbita {
          0%   { transform: translate(0px, 0px);   opacity: 1; }
          40%  { transform: translate(140px, 15px); opacity: 1; }
          50%  { transform: translate(140px, 15px); opacity: 1; }
          70%  { transform: translate(75px, 5px);   opacity: 0; }
          90%  { transform: translate(10px, 0px);   opacity: 0; }
          100% { transform: translate(0px, 0px);    opacity: 1; }
        }
        @keyframes dspLogoFlotar {
          0%, 100% { transform: translateY(0px); }
          50%      { transform: translateY(-6px); }
        }
        .dsp-logo-esfera {
          animation: dspLogoOrbita 5s ease-in-out infinite;
          transform-origin: 45.5px 42.5px;
          will-change: transform, opacity;
        }
        .dsp-logo-planeta {
          animation: dspLogoFlotar 4s ease-in-out infinite;
          transform-origin: 124px 114px;
          will-change: transform;
        }
        .dsp-logo-texto {
          font-family: 'Agbalumo', cursive;
        }
      `}</style>

      <defs>
        <linearGradient id="dspLogoStroke" x1="0.5" y1="0" x2="0.5" y2="1">
          <stop offset="0" stopColor="#42c4e7" stopOpacity="0.99" />
          <stop offset="1" stopColor="#7e42e7" stopOpacity="0.584" />
        </linearGradient>
        <linearGradient id="dspLogoPlanetGrad" x1="0.5" y1="0" x2="0.5" y2="1">
          <stop offset="0.2" stopColor="#154db5" />
          <stop offset="1" stopColor="#a102d6" />
        </linearGradient>
        <filter id="dspLogoShadowSoft" x="-50%" y="-50%" width="200%" height="200%">
          <feDropShadow dx="4" dy="4" stdDeviation="2" floodColor="#000" floodOpacity="0.2" />
        </filter>
        <filter id="dspLogoShadowHard" x="-100%" y="-100%" width="300%" height="300%">
          <feDropShadow dx="3" dy="3" stdDeviation="2" floodColor="#000" floodOpacity="0.4" />
        </filter>
      </defs>

      {/* Triángulo principal (borde degradado azul → morado, punta afilada) */}
      <path
        d="M124,0L16,185L232,185L124,0"
        fill="none"
        stroke="url(#dspLogoStroke)"
        strokeWidth="30"
        strokeLinejoin="miter"
        strokeMiterlimit={10}
      />

      {/* Base amarilla + rótulo "DSP AudioPad" (capa estática) */}
      <g>
        <path
          d="M10.25,188.96L249,149L216.78,223L0,223L10.25,188.96"
          fill="#FFE100"
        />
        <text
          className="dsp-logo-texto"
          x="20.4"
          y="226.8"
          transform="rotate(-9.7 20.4 226.8)"
          fontSize="30"
          fill="#B8C6C8"
          filter="url(#dspLogoShadowSoft)"
        >
          DSP AudioPad
        </text>
      </g>

      {/* Planeta con anillo (flota) */}
      <g className="dsp-logo-planeta">
        <ellipse cx="124" cy="114" rx="55" ry="55" fill="url(#dspLogoPlanetGrad)" />
        <path
          d="M170.8,85.1C170.2,84.2,169.6,83.3,169,82.4C185.8,78.8,197.5,79.1,199.7,84C203.6,93.2,173.1,114.9,131.6,132.5C90.1,150.2,53.3,157.1,49.3,148C47.2,143,55.4,134.2,70,124.4C70.2,125.5,70.4,126.6,70.7,127.7C65.9,132.1,63.5,135.7,64.6,138.2C67.5,145,96.2,139.3,128.7,125.4C161.2,111.6,185.2,94.7,182.3,87.9C181.3,85.5,177.1,84.6,170.8,85.1"
          fill="#42d3e7"
          stroke="#42d3e7"
          strokeWidth="14"
          filter="url(#dspLogoShadowSoft)"
        />
      </g>

      {/* Esfera gris que orbita */}
      <g className="dsp-logo-esfera">
        <ellipse
          cx="45.5"
          cy="42.5"
          rx="14.5"
          ry="14.5"
          fill="#b1b2b5"
          filter="url(#dspLogoShadowHard)"
        />
      </g>
    </svg>
  )
}
