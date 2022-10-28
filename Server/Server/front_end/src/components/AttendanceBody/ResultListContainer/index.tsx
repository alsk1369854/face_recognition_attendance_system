import React, { useState } from 'react'

import style from './index.module.scss'

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faSort } from '@fortawesome/free-solid-svg-icons' // <-- import styles to be used

import TableItem from './TableItem'
import type { iAttendanceRecord } from '..'

interface iResultListContainer {
  attendanceRecordQueryResult: iAttendanceRecord[],
  setAttendanceRecordQueryResult: Function
}

export default function ResultListContainer({
  attendanceRecordQueryResult,
  setAttendanceRecordQueryResult
}: iResultListContainer) {

  const [count, setCount] = useState(0)

  const renderComponent = () => {
    setCount(count+1)
  }

  const sortTableOnClick = (sortBy: string) => {
    let newData = attendanceRecordQueryResult
    switch (sortBy) {
      case 'person_id':
        newData = newData.sort((a1: iAttendanceRecord, a2: iAttendanceRecord) => {
          return a1.person._id - a2.person._id
        })
        break;
      case 'person_name':
        newData = newData.sort((a1: iAttendanceRecord, a2: iAttendanceRecord) => {
          return a1.person.name.localeCompare(a2.person.name)
        })
        break;
      case 'date_time_record':
        newData = newData.sort((a1: iAttendanceRecord, a2: iAttendanceRecord) => {
          return a1.date_time_stamp.localeCompare(a2.date_time_stamp)
        })
        break
    }
    setAttendanceRecordQueryResult(newData)
    renderComponent()
  }

  const getTableItems = () => {
    let result = []
    for (let i = 0; i < attendanceRecordQueryResult.length; i++) {
      const attendanceRecord = attendanceRecordQueryResult[i]
      result.push(
        <TableItem
          key={`AttendanceTableItem_${i}`}
          number={i + 1}
          officeName={attendanceRecord.room.name}
          personId={attendanceRecord.person._id}
          personName={attendanceRecord.person.name}
          attendanceRecord={attendanceRecord.date_time_stamp}
        />
      )
    }
    return result
  }

  return (
    <div className={style.result_list_container}>
      <div className={style.title_container}>
        <div className={style.title_header}>
          查詢結果
        </div>
        <div>
          {`總共: ${attendanceRecordQueryResult.length} 筆數據`}
        </div>
      </div>
      <div className={style.table_header}>
        <div className={style.table_header_number}>
          編號
        </div>
        <div className={style.divider}>|</div>
        <div className={style.table_header_office_name}>
          辦公室名稱
        </div>
        <div className={style.divider}>|</div>
        <div
          className={style.table_header_person_id}
          onClick={() => sortTableOnClick('person_id')}
        >
          <span className={style.table_header_sort_icon}>
            <FontAwesomeIcon icon={faSort} />
          </span>
          人員ID
        </div>
        <div className={style.divider}>|</div>
        <div
          className={style.table_header_person_name}
          onClick={() => sortTableOnClick('person_name')}
        >
          <span className={style.table_header_sort_icon}>
            <FontAwesomeIcon icon={faSort} />
          </span>
          人員名稱
        </div>
        <div className={style.divider}>|</div>
        <div
          className={style.table_header_attendance_record}
          onClick={() => sortTableOnClick('date_time_record')}
        >
          <span className={style.table_header_sort_icon}>
            <FontAwesomeIcon icon={faSort} />
          </span>
          出勤時間
        </div>
      </div>

      <div className={style.table_item_container}>
        {getTableItems()}
      </div>
    </div>
  )
}
