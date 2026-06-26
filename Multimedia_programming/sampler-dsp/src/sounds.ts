// ──────────────────────────────────────────────
//  DSP sound generators — cada función retorna un
//  AudioBuffer sintetizado directamente en memoria.
// ──────────────────────────────────────────────

import { VOZ_SAMPLE_DATA_URI } from './voiceAsset'

// Decodifica el sample de voz (audio real, no sintetizado) embebido
// como data URI en base64, usando AudioContext.decodeAudioData.
export async function loadVozSample(ctx: AudioContext): Promise<AudioBuffer> {
  const res = await fetch(VOZ_SAMPLE_DATA_URI)
  const arrayBuffer = await res.arrayBuffer()
  return ctx.decodeAudioData(arrayBuffer)
}

export function genKick(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.6, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    const freq = 60 + 150 * Math.exp(-t * 18)
    d[i] = Math.sin(2 * Math.PI * freq * t) * Math.exp(-t * 9) * 0.9
  }
  return buf
}

export function genSnare(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.35, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    const noise = Math.random() * 2 - 1
    const tone = Math.sin(2 * Math.PI * 210 * t)
    d[i] = (noise * 0.65 + tone * 0.35) * Math.exp(-t * 14) * 0.75
  }
  return buf
}

export function genHHclosed(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.08, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    d[i] = (Math.random() * 2 - 1) * Math.exp(-t * 70) * 0.55
  }
  return buf
}

export function genHHopen(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.5, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    d[i] = (Math.random() * 2 - 1) * Math.exp(-t * 3.5) * 0.5
  }
  return buf
}

export function genClap(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.25, sr)
  const d = buf.getChannelData(0)
  const bursts = [0, 0.008, 0.018, 0.035]
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    let v = 0
    for (const b of bursts) {
      const dt = t - b
      if (dt >= 0) v += (Math.random() * 2 - 1) * Math.exp(-dt * 90)
    }
    d[i] = v * 0.38
  }
  return buf
}

export function genTom(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.45, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    const freq = 90 + 50 * Math.exp(-t * 12)
    d[i] = Math.sin(2 * Math.PI * freq * t) * Math.exp(-t * 7) * 0.82
  }
  return buf
}

export function genBass(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.6, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    const v =
      Math.sin(2 * Math.PI * 50 * t) +
      0.3 * Math.sin(2 * Math.PI * 100 * t)
    d[i] = (v / 1.3) * Math.exp(-t * 4.5) * 0.85
  }
  return buf
}

export function genSynth(ctx: AudioContext): AudioBuffer {
  const sr = ctx.sampleRate
  const buf = ctx.createBuffer(1, sr * 0.4, sr)
  const d = buf.getChannelData(0)
  for (let i = 0; i < d.length; i++) {
    const t = i / sr
    const v =
      Math.sin(2 * Math.PI * 440 * t) +
      0.5 * Math.sin(2 * Math.PI * 880 * t) +
      0.25 * Math.sin(2 * Math.PI * 1320 * t)
    d[i] = (v / 1.75) * Math.exp(-t * 5.5) * 0.7
  }
  return buf
}
