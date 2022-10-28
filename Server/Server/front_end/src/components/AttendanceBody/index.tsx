import React, { useState } from 'react'

import style from './index.module.css'

import SearchBar from './SearchBar'
import ResultListContainer from './ResultListContainer'

export interface iRoom {
  _id: number,
  name: string,
  webcam_device_id: string
}

export interface iPeron {
  _id: number,
  name: string
}

export interface iAttendanceRecord {
  _id: number,
  room: iRoom,
  person: iPeron,
  date_time_stamp: string
}

export default function AttendanceBody() {
  const [attendanceRecordQueryResult, setAttendanceRecordQueryResult] = useState<iAttendanceRecord[]>([])

  return (
    <div className={style.attendance_body}>
      <SearchBar
        setAttendanceRecordQueryResult={setAttendanceRecordQueryResult}
      />

      <ResultListContainer
        attendanceRecordQueryResult={attendanceRecordQueryResult}
        setAttendanceRecordQueryResult={setAttendanceRecordQueryResult}
      />
    </div>
  )
}
