import * as dotenv from 'dotenv';
dotenv.config();

import express from 'express';
import pageRouter from './routers/pageRouter';
import apiRouter from './routers/apiRouter';
import { getCards } from './db/cards';

const path = require('path')

const app = express()

app.set('view engine', 'ejs');

app.set('views', path.join(__dirname, 'views'));

app.use(express.urlencoded({ extended: true }));

app.use('/', pageRouter);
app.use('/api', apiRouter);

app.listen(3001, () => {
    console.log("HTTP Server listening on :3001...")
})

getCards()
