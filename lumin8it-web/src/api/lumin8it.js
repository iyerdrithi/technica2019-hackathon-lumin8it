import axios from "axios";

const instance = axios.create({
  baseURL: "https://api.lumin8it.com/v1"
});

instance.interceptors.request.use();

export default instance;
