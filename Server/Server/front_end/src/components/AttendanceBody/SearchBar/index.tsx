import React, { useState, useEffect } from 'react'

import style from './index.module.scss'

import moment from 'moment';
import { DownloadOutlined, SearchOutlined } from '@ant-design/icons';
import { Select, DatePicker, Button } from 'antd';
import type { RangePickerProps } from 'antd/es/date-picker';

import { getAllRoom, getRoomRecordsBetweenTwoDatetime } from '../../../utils/apiUtil'
import type { iRoomRecordsBetweenTwoDatetime } from '../../../utils/apiUtil'

const { Option } = Select;
const { RangePicker } = DatePicker;


const disabledDate: RangePickerProps['disabledDate'] = current => {
  return current && current > moment().endOf('day');
};


interface iRoom {
  _id: number,
  name: string,
  webcam_device_id: string
}

interface iPeron {
  _id: number,
  name: string
}

interface iAttendanceRecord {
  _id: number,
  room: iRoom,
  person: iPeron,
  date_time_stamp: string
}

export default function SearchBar() {
  const [count, setCount] = useState<number>(0);
  const [officeRoomNameList, setOfficeRoomNameList] = useState<iRoom[]>([]);
  const [officeRoomPickerData, setOfficeRoomPickerData] = useState<string | undefined>();
  const [datetimeRangePickerData, setDatetimeRangePickerOnChange] = useState<string[] | undefined>();
  const [commitButtonLoadings, setCommitButtonLoadings] = useState<boolean[]>([]);
  const [attendanceRecordQueryResult, setAttendanceRecordQueryResult] = useState<iAttendanceRecord[]>([])

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
        end_datetime: datetimeRangePickerData[1]
      }

      getRoomRecordsBetweenTwoDatetime(values)
        .then((response) => {
          const data = response.data
          console.log(data)
          switch (index) {
            case 1: // 下載
              
            case 0: // 查詢
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
            style={{ width: 120 }}
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
            // defaultPickerValue={}
            disabledDate={disabledDate}
            showTime
            onChange={datetimeRangePickerOnChange}
          />
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
