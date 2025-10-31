import { pool } from "../db"
import { State, StateDto } from "../types/State"

export async function createState(uid: string): Promise<StateDto | null> {
    let [result] = await pool.query(
        'INSERT INTO state (uid, state) VALUES(?, 0)',
        [uid]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return null
    }
    return {
        uid,
        state: false
    }
}

export async function getState(uid: string): Promise<StateDto | null> {
    const [rows] = await pool.query(
        'SELECT * FROM state WHERE uid = ?',
        [uid]
    )

    if (rows[0]) {
        return {
            uid: rows[0].uid,
            state: rows[0].state[0] === 1
        }
    }
    return null

}

export async function setState(uid: string, state: boolean): Promise<boolean> {
    let [result] = await pool.query(
        'UPDATE state SET state = ? WHERE uid = ?',
        [state ? 1 : 0, uid]
    )

    const info = result as any;

    if (info.affectedRows === 0) {
        return false
    }
    return true
}