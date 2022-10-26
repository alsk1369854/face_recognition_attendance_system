import React from 'react'

import style from './index.module.css'

import AttendanceHeader from '../../components/AttendanceHeader'
import AttendanceBody from '../../components/AttendanceBody'

export default function AttendancePage() {
  return (
    <div className={style.attendance_page}>
      <AttendanceHeader/>
      <AttendanceBody/>
    </div>
  )
}
