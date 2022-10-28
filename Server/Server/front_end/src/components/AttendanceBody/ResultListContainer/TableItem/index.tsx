import React from 'react'

import style from './index.module.scss'

interface iTableItem {
    number: number | string,
    officeName: string,
    personId: number | string,
    personName: string,
    attendanceRecord: string,
}

export default function TableItem({
    number,
    officeName,
    personId,
    personName,
    attendanceRecord
}: iTableItem) {
    return (
        <>
            <div className={style.table_item}>
                <div className={style.table_item_number}>
                    {number}
                </div>
                <div className={style.divider}>|</div>
                <div className={style.table_item_office_name}>
                    {officeName}
                </div>
                <div className={style.divider}>|</div>
                <div className={style.table_item_person_id}>
                    {personId}
                </div>
                <div className={style.divider}>|</div>
                <div className={style.table_item_person_name}>
                    {personName}
                </div>
                <div className={style.divider}>|</div>
                <div className={style.table_item_attendance_record}>
                    {attendanceRecord}
                </div>
            </div>
            <div className={style.bottom_line}></div>
        </>
    )
}
