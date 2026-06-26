import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

export default defineConfig({
  plugins: [react()],
  // Rutas relativas en el build final: esto hace que la app funcione tanto
  // si se sirve desde la raíz de un dominio (Netlify, Vercel) como desde una
  // subcarpeta (ej. GitHub Pages: https://usuario.github.io/nombre-repo/).
  base: './',
})
