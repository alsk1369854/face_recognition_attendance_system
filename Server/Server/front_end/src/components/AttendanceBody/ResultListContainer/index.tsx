import React from 'react'

import style from './index.module.scss'

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faSort } from '@fortawesome/free-solid-svg-icons' // <-- import styles to be used


export default function ResultListContainer() {
  return (
    <div className={style.result_list_container}>
      <div className={style.title_container}>
        <div className={style.title_header}>
          查詢結果
        </div>
      </div>
      <div className={style.table_header}>
        <div className={style.talbe_header_office_name}>
          office_name
        </div>
        <div className={style.table_header_person_id}>
          <FontAwesomeIcon icon={faSort} />
          person_id
        </div>
        <div className={style.table_header_person_name}>
          person_name
        </div>
        <div className={style.table_header_attendance_record}>
          attendance_record
        </div>
      </div>
      <div className={style.table_item_container}>
        <div className={style.table_item}>
          itme1
        </div>

      </div>
    </div>
  )
}
