import React from 'react'

import style from './index.module.css'

import SearchBar from './SearchBar'
import ResultListContainer from './ResultListContainer'

export default function AttendanceBody() {
  return (
    <div className={style.attendance_body}>
        <SearchBar/>
        <ResultListContainer/>
    </div>
  )
}
