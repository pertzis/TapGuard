import { pool } from "../db";
import { Card, CardDto } from "../types/Card";

export async function createCard(card: CardDto): Promise<boolean> {
    let [result] = await pool.query(
        'INSERT INTO cards (uid, name, active) VALUES(?, ?, ?)',
        [card.uid, card.name, card.active ? 1 : 0]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return false    
    }
    return true
}

export async function getCards(): Promise<CardDto[]> {
    const [rows] = await pool.query(
        'SELECT * FROM cards'
    )

    const cards: CardDto[] = rows.map((row: Card) => (
        {
            uid: row.uid,
            name: row.name,
            active: row.active[0] === 1
        }
    ))

    return cards

}

export async function getCard(uid: string): Promise<CardDto | null> {
    const [rows] = await pool.query(
        'SELECT * FROM cards WHERE uid = ?',
        [uid]
    )

    if (rows[0]) {
        return {
            uid: rows[0].uid,
            name: rows[0].name,
            active: rows[0].active[0] === 1
        }
    }
    return null

}

export async function updateCard(uid: string, card: CardDto): Promise<boolean> {
    let [result] = await pool.query(
        'UPDATE cards SET uid = ?, name = ?, active = ? WHERE uid = ?',
        [card.uid, card.name, card.active ? 1 : 0, uid]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return false
    }
    return true

}

export async function deleteCard(uid: string): Promise<boolean> {
    let [result] = await pool.query(
        'DELETE FROM users WHERE uid = ?',
        [uid]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return false
    }
    return true
}