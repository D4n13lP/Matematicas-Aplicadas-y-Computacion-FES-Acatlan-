const sqlite3 = require('sqlite3').verbose();
const path = require('path');

// Crear o conectar a la base de datos
const db2 = new sqlite3.Database(path.resolve(__dirname, 'dailyfit_v2.db'), (err) => {
  if (err) return console.error('Error al conectar con SQLite:', err.message);
  console.log('Conectado a la base de datos SQLite ✅');
});

module.exports = db2;