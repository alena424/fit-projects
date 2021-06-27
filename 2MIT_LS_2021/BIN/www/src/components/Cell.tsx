import React from 'react';
import { CellType, OnClickCell } from "../models/cellModel";

type CellProps = {
    index: number
    cell: CellType
    onClick?: OnClickCell
}

const Cell: React.FC<CellProps> = props => {
    const {cell, onClick, index} = props

    return (
        <div
            className="cell"
            style={cell.active ? {background: "black"} : {background: "white"}}
            onClick={() => onClick ? onClick(cell, index) : console.log("you can change cells only in the first iteration")}
        />
    )
};

export default Cell;
