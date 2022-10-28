import axios from 'axios'

const userRequest = axios.create({
  baseURL: 'http://192.168.1.37:8888',
  // baseURL: 'http://localhost:8888',
  headers: { 'Content-Type': 'application/json' },
})

export interface iRoomRecordsBetweenTwoDatetime {
  room_name: string,
  start_datetime: string,
  end_datetime: string,
  order_by?: string
}

export const postFunc = (data: any) => userRequest.post('/users', data)
export const getFunc = (url: string) => userRequest.get(url)

export const getAllRoom = () => getFunc('/attendance?operate=get_all_room')
export const getRoomRecordsBetweenTwoDatetime = (values: any) => {
  const url = (values.order_by) ?
    `/attendance?operate=search&room_name=${values.room_name}&start_datetime=${values.start_datetime}&end_datetime=${values.end_datetime}&order_by=${values.order_by}`
    : `/attendance?operate=search&room_name=${values.room_name}&start_datetime=${values.start_datetime}&end_datetime=${values.end_datetime}`
  return getFunc(url)
}