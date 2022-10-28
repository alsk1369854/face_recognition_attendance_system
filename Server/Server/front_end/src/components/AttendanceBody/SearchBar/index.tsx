import React, { useState, useEffect, FC } from 'react'

import style from './index.module.scss'

import moment from 'moment';
import { DownloadOutlined, SearchOutlined } from '@ant-design/icons';
import { Select, DatePicker, Button } from 'antd';
import type { RangePickerProps } from 'antd/es/date-picker';

import { getAllRoom, getRoomRecordsBetweenTwoDatetime } from '../../../utils/apiUtil'
import { saveTextFile } from '../../../utils/saveFileUtil';
import type { iRoomRecordsBetweenTwoDatetime } from '../../../utils/apiUtil'

import type { iRoom, iPeron, iAttendanceRecord } from '..';

const { Option } = Select;
const { RangePicker } = DatePicker;


const disabledDate: RangePickerProps['disabledDate'] = current => {
  return current && current > moment().endOf('day');
};

const downloadCSVFile = (attendanceRecordList: iAttendanceRecord[]) => {
  const fileName = 'attendance_record.csv'
  let fileContent = 'No., Room Name, Person ID, Person Name, Date Time Record\n'
  for (let i = 0; i < attendanceRecordList.length; i++) {
    fileContent += `${i + 1}, ${attendanceRecordList[i].room.name}, ${attendanceRecordList[i].person._id}, ${attendanceRecordList[i].person.name}, ${attendanceRecordList[i].date_time_stamp}\n`
  }
  saveTextFile(fileName, fileContent)
}


interface iSearchBarProps {
  setAttendanceRecordQueryResult: Function
}

export default function SearchBar({
  setAttendanceRecordQueryResult
}: iSearchBarProps) {

  const [count, setCount] = useState<number>(0);
  const [officeRoomNameList, setOfficeRoomNameList] = useState<iRoom[]>([]);
  const [officeRoomPickerData, setOfficeRoomPickerData] = useState<string | undefined>();
  const [datetimeRangePickerData, setDatetimeRangePickerOnChange] = useState<string[] | undefined>();
  const [orderPickerData, setOrderPickerData] = useState<string>('person')
  const [commitButtonLoadings, setCommitButtonLoadings] = useState<boolean[]>([]);

  useEffect(() => {
    // loading Room Name to show
    if (officeRoomNameList.length === 0) {
      loadOfficeRoomNameList()
    }
    return () => { }
  });

  const renderComponent = () => {
    setCount(count + 1);
  }

  const loadOfficeRoomNameList = () => {
    getAllRoom()
      .then((response) => {
        const data = response.data
        setOfficeRoomNameList(data)
      })
      .catch((error) => {
        console.error('Error \nAttendanceBody > SearchBar: Func=>loadOfficeRoomNameList: ', error);
      })
  }

  const officeRoomPickerOnChange = (value: string) => {
    setOfficeRoomPickerData(value)
  };

  const datetimeRangePickerOnChange = (data: any, string: string[]) => {
    setDatetimeRangePickerOnChange(string)
  };

  const orderPickerOnChange = (value: string) => {
    setOrderPickerData(value)
  };

  const setCommitButtonStartLoading = (index: number) => {
    setCommitButtonLoadings(prevLoadings => {
      const newLoadings = [...prevLoadings];
      newLoadings[index] = true;
      return newLoadings;
    });
  }

  const setCommitButtonEndLoading = (index: number) => {
    setCommitButtonLoadings(prevLoadings => {
      const newLoadings = [...prevLoadings];
      newLoadings[index] = false;
      return newLoadings;
    });
  }

  const commitButtonOnClick = (index: number) => {
    // index 0: "查詢" On click
    // index 1: "下載" On click

    if (officeRoomPickerData && datetimeRangePickerData) {
      // Start commit button loading
      setCommitButtonStartLoading(index)

      const values: iRoomRecordsBetweenTwoDatetime = {
        room_name: officeRoomPickerData,
        start_datetime: datetimeRangePickerData[0],
        end_datetime: datetimeRangePickerData[1],
        order_by: (orderPickerData == 'person_name') ? "person_id" : orderPickerData,
      }

      getRoomRecordsBetweenTwoDatetime(values)
        .then((response) => {
          let data = response.data

          if (orderPickerData === 'person_name') {
            data = data.sort((a1: iAttendanceRecord, a2: iAttendanceRecord) => {
              return a1.person.name.localeCompare(a2.person.name)
            })
          }

          switch (index) {
            case 0: // 查詢
              setAttendanceRecordQueryResult(data)
              break;
            case 1: // 下載
              downloadCSVFile(data)
              setAttendanceRecordQueryResult(data)
              break;
          }
        })
        .catch((error) => {
          console.error(`Error \nAttendanceBody > SearchBar: Func=>commitButtonOnClick index=${index}: ${error}`)
        })
        .finally(() => {
          setCommitButtonEndLoading(index)
        })
    } else {
      // Picker not finished
    }
  };

  return (
    <div className={style.search_bar}>
      <div className={style.search_item_container}>
        <div className={style.select_item}>
          <span className={style.select_item_name}>
            辦公室名稱:
          </span>
          <Select
            // defaultValue=""
            style={{ width: 90 }}
            onChange={officeRoomPickerOnChange}
          >
            {
              officeRoomNameList.map((roomItem) => {
                return <Option
                  key={`officeRoomNameItem_${roomItem._id}`}
                  value={roomItem.name}>{roomItem.name}
                </Option>
              })
            }
          </Select>
        </div>

        <div className={style.select_item}>
          <span className={style.select_item_name}>
            查詢時段:
          </span>
          <RangePicker
            style={{ width: 370 }}
            // defaultPickerValue={}
            disabledDate={disabledDate}
            showTime
            onChange={datetimeRangePickerOnChange}
          />
        </div>

        <div className={style.select_item}>
          <span className={style.select_item_name}>
            排序:
          </span>
          <Select
            defaultValue="人員ID"
            style={{ width: 100 }}
            onChange={orderPickerOnChange}
          >
            <Option value={'person'}>人員ID</Option>
            <Option value={'person_name'}>人員名稱</Option>
            <Option value={'date_time_stamp'}>出勤時間</Option>
          </Select>
        </div>
      </div>



      <div className={style.commit_button_container}>
        <div className={style.commit_button}>
          <Button
            type="primary"
            icon={<SearchOutlined />}
            loading={commitButtonLoadings[0]}
            onClick={() => commitButtonOnClick(0)}
          >
            查詢
          </Button>
        </div>

        <div className={style.commit_button}>
          <Button
            type="primary"
            icon={<DownloadOutlined />}
            loading={commitButtonLoadings[1]}
            onClick={() => commitButtonOnClick(1)}
          >
            下載
          </Button>
        </div>
      </div>

    </div>
  )
}
