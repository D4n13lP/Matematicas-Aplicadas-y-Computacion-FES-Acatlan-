const express = require('express');
const cors = require('cors');
const db = require('./db2');
const path = require('path');

const app = express();
const PORT = 3000;

app.use(cors());
app.use(express.json());

// Servir archivos estáticos (CSS, JS, imágenes) desde la carpeta client
app.use(express.static(path.join(__dirname, '../client')));


app.get('/dailyFit', (req, res) => {
  res.sendFile(path.join(__dirname, '../client/index.html'));
});

// Insertar progreso
app.post('/api/progress', (req, res) => {
  const { ID_Usuario, ID_Ejercicio, Semana, Repeticiones, Peso } = req.body;

  // Validación básica
  if (!ID_Usuario || !ID_Ejercicio || !Semana || Repeticiones == null || Peso == null) {
    return res.status(400).json({ error: 'Todos los campos son requeridos' });
  }

  const stmt = `
    INSERT INTO Progreso (ID_Usuario, ID_Ejercicio, Semana, Repeticiones, Peso)
    VALUES (?, ?, ?, ?, ?)
  `;

  db.run(stmt, [ID_Usuario, ID_Ejercicio, Semana, Repeticiones, Peso], function (err) {
    if (err) {
      console.error(err.message);
      return res.status(500).json({ error: 'Error al guardar el progreso' });
    }

    res.status(201).json({
      ID_Progreso: this.lastID,
      ID_Usuario,
      ID_Ejercicio,
      Semana,
      Repeticiones,
      Peso
    });
  });
});


// Insertar grupo muscular
app.post('/api/grupo-muscular', (req, res) => {
  const { Nombre_grupo_Muscular } = req.body;
  if (!Nombre_grupo_Muscular) return res.status(400).json({ error: 'El nombre del grupo muscular es requerido' });

  const stmt = `INSERT INTO Grupo_muscular (Nombre_grupo_Muscular) VALUES (?)`;
  db.run(stmt, [Nombre_grupo_Muscular], function (err) {
    if (err) return res.status(500).json({ error: 'Error al guardar grupo muscular' });
    res.status(201).json({ ID_Grupo: this.lastID, Nombre_grupo_Muscular });
  });
});

// Insertar ejercicio
app.post('/api/ejercicio', (req, res) => {
  const { ID_Grupo, Nombre_ejercicio } = req.body;
  if (!ID_Grupo || !Nombre_ejercicio) return res.status(400).json({ error: 'Todos los campos son requeridos' });

  const stmt = `INSERT INTO Ejercicio (ID_Grupo, Nombre_ejercicio) VALUES (?, ?)`;
  db.run(stmt, [ID_Grupo, Nombre_ejercicio], function (err) {
    if (err) return res.status(500).json({ error: 'Error al guardar ejercicio' });
    res.status(201).json({ ID_Ejercicio: this.lastID, ID_Grupo, Nombre_ejercicio });
  });
});

// Insertar usuario
app.post('/api/usuario', (req, res) => {
  const { Nombre, Genero, Edad, Telefono, Correo } = req.body;
  if (!Nombre || !Genero || Edad == null) return res.status(400).json({ error: 'Nombre, Género y Edad son requeridos' });

  const stmt = `
    INSERT INTO Usuario (Nombre, Genero, Edad, Telefono, Correo)
    VALUES (?, ?, ?, ?, ?)
  `;
  db.run(stmt, [Nombre, Genero, Edad, Telefono, Correo], function (err) {
    if (err) return res.status(500).json({ error: 'Error al guardar usuario' });
    res.status(201).json({ ID_Usuario: this.lastID, Nombre, Genero, Edad, Telefono, Correo });
  });
});


// -------------------------------------------- GET ---------------------------------------------------

// Obtener todos los usuarios
app.get('/api/usuarios', (req, res) => {
  db.all('SELECT * FROM Usuario', [], (err, rows) => {
    if (err) return res.status(500).json({ error: 'Error al obtener usuarios' });
    res.json(rows);
  });
});

// Obtener usuario por correo para login
app.post('/api/login', (req, res) => {
  const { correo } = req.body;

  if (!correo) {
    return res.status(400).json({ error: 'Se requiere el correo' });
  }

  db.get('SELECT * FROM Usuario WHERE correo = ?', [correo], (err, row) => {
    if (err) {
      return res.status(500).json({ error: 'Error al obtener el usuario' });
    }
    if (!row) {
      return res.status(404).json({ error: 'Usuario no encontrado' });
    }

    res.json(row);
  });
});

// Obtener todos los grupos musculares
app.get('/api/grupo-muscular', (req, res) => {
  db.all('SELECT * FROM Grupo_muscular', [], (err, rows) => {
    if (err) return res.status(500).json({ error: 'Error al obtener grupos musculares' });
    res.json(rows);
  });
});

// Obtener todos los ejercicios
app.get('/api/ejercicios', (req, res) => {
  db.all('SELECT * FROM Ejercicio', [], (err, rows) => {
    if (err) return res.status(500).json({ error: 'Error al obtener ejercicios' });
    res.json(rows);
  });
});

// Obtener todo el progreso
app.get('/api/progress', (req, res) => {
  const query = `
    SELECT 
      P.ID_Progreso,
      U.Nombre AS Usuario,
      E.Nombre_ejercicio AS Ejercicio,
      GM.Nombre_grupo_Muscular AS Grupo_muscular,
      P.Semana,
      P.Repeticiones,
      P.Peso
    FROM Progreso P
    JOIN Usuario U ON P.ID_Usuario = U.ID_Usuario
    JOIN Ejercicio E ON P.ID_Ejercicio = E.ID_Ejercicio
    JOIN Grupo_muscular GM ON E.ID_Grupo = GM.ID_Grupo
    ORDER BY P.ID_Progreso DESC
  `;

  db.all(query, [], (err, rows) => {
    if (err) return res.status(500).json({ error: 'Error al obtener el progreso' });
    res.json(rows);
  });
});

// Obtener usuario por correo (usado como fallback)
app.get('/api/usuario', (req, res) => {
  const correo = req.query.correo;

  if (!correo) {
    return res.status(400).json({ error: 'Correo requerido' });
  }

  db.get('SELECT ID_Usuario FROM Usuario WHERE correo = ?', [correo], (err, row) => {
    if (err) {
      return res.status(500).json({ error: 'Error al buscar el usuario' });
    }
    if (!row) {
      return res.status(404).json({ error: 'Usuario no encontrado' });
    }

    res.json(row);
  });
});
// -------------------------------------------- FIN GET ---------------------------------------------------

//--------------------------------------------- PUT -------------------------------------------------------

app.put('/api/usuario/:id', (req, res) => {
  const { id } = req.params;
  const { Nombre, Genero, Edad, Telefono, Correo } = req.body;

  const stmt = `
    UPDATE Usuario 
    SET Nombre = ?, Genero = ?, Edad = ?, Telefono = ?, Correo = ?
    WHERE ID_Usuario = ?
  `;
  db.run(stmt, [Nombre, Genero, Edad, Telefono, Correo, id], function (err) {
    if (err) return res.status(500).json({ error: 'Error al actualizar usuario' });
    res.json({ message: 'Usuario actualizado' });
  });
});

app.put('/api/ejercicio/:id', (req, res) => {
  const { id } = req.params;
  const { ID_Grupo, Nombre_ejercicio } = req.body;

  const stmt = `
    UPDATE Ejercicio 
    SET ID_Grupo = ?, Nombre_ejercicio = ?
    WHERE ID_Ejercicio = ?
  `;
  db.run(stmt, [ID_Grupo, Nombre_ejercicio, id], function (err) {
    if (err) return res.status(500).json({ error: 'Error al actualizar ejercicio' });
    res.json({ message: 'Ejercicio actualizado' });
  });
});

app.put('/api/progress/:id', (req, res) => {
  const { id } = req.params;
  const { ID_Usuario, ID_Ejercicio, Semana, Repeticiones, Peso } = req.body;

  const stmt = `
    UPDATE Progreso 
    SET ID_Usuario = ?, ID_Ejercicio = ?, Semana = ?, Repeticiones = ?, Peso = ?
    WHERE ID_Progreso = ?
  `;
  db.run(stmt, [ID_Usuario, ID_Ejercicio, Semana, Repeticiones, Peso, id], function (err) {
    if (err) return res.status(500).json({ error: 'Error al actualizar progreso' });
    res.json({ message: 'Progreso actualizado' });
  });
});

//-------------------------------------------- FIN PUT -----------------------------------------------------

//--------------------------------------------- DELETE -----------------------------------------------------
app.delete('/api/usuario/:id', (req, res) => {
  db.run('DELETE FROM Usuario WHERE ID_Usuario = ?', [req.params.id], function (err) {
    if (err) return res.status(500).json({ error: 'Error al eliminar usuario' });
    res.json({ message: 'Usuario eliminado' });
  });
});
//------------------------------------------- FIN DELETE ---------------------------------------------------

app.listen(PORT, () => {
  console.log(`Servidor corriendo en http://localhost:${PORT}`);
});
