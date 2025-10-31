export interface Card {
    uid: string,
    name: string,
    active: Buffer
}
export interface CardDto {
    uid: string,
    name: string,
    active: boolean
}