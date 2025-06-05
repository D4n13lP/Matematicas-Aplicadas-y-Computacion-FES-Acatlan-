const sqlite3 = require('sqlite3').verbose();
const path = require('path');

// Crear o conectar a la base de datos
const db = new sqlite3.Database(path.resolve(__dirname, 'dailyfit.db'), (err) => {
  if (err) return console.error('Error al conectar con SQLite:', err.message);
  console.log('Conectado a la base de datos SQLite ✅');
});

// Crear tabla si no existe
db.serialize(() => {
  db.run(`
    CREATE TABLE IF NOT EXISTS progress (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      exercise TEXT NOT NULL,
      value REAL NOT NULL,
      unit TEXT NOT NULL,
      date TEXT DEFAULT CURRENT_TIMESTAMP
    )
  `);
});

module.exports = db;